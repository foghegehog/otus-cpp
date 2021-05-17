#include <atomic>
#include <mutex>
#include <thread>

#include "../include/commands_parsing.h"
#include "../include/context.h"
#include "../include/handlers_factory.h"
#include "../include/handlers/accumulate_handler.h"
#include "../include/handlers/control_unit_handler.h"
#include "../include/handlers/handlers_chain.h"
#include "../include/handlers/processing_handler.h"


namespace async{

using namespace postprocessing;
using namespace std;


thread Context::s_logger_thread1 = thread(
            &Postprocessing::Run,
            Postprocessing(make_unique<LoggingHandler>("file1"), s_logging_queue));
thread Context::s_logger_thread2 = thread(
            &Postprocessing::Run,
            Postprocessing(make_unique<LoggingHandler>("file2"), s_logging_queue));

thread Context::s_output_thread = thread(
            &Postprocessing::Run, Postprocessing(make_unique<OutputHandler>(), s_output_queue));

static std::shared_ptr<handlers::ThreadSafeAccumulator> get_static_commands_accumulator()
{
    static std::shared_ptr<handlers::ThreadSafeAccumulator> instance = std::make_shared<handlers::ThreadSafeAccumulator>();                               
    return instance;
}

Context::Context(size_t bulk_size)
{
    m_dynamic_accumulator = std::make_shared<handlers::Accumulator>(); 
    m_static_accumulator = get_static_commands_accumulator();
    m_control_unit = std::make_shared<handlers::ControlUnit>(bulk_size);
    m_handlers = std::move(create_handlers_chain(m_static_accumulator, m_dynamic_accumulator, m_control_unit));
}

size_t Context::read_buffer_blocking(const char * buffer, size_t chars_count)
{
    const char * buffer_end = buffer + chars_count - 1;
    const char separator = '\n';
    const char * start = commands_parsing::skip_separator(buffer, buffer_end, separator);

    auto is_finish = [buffer_end](const char * pos)
    { 
        return commands_parsing::is_buffer_end(pos, buffer_end); 
    };

    auto generator = [separator, buffer_end](const char *& pos){
        return commands_parsing::extract_command(pos, buffer_end, separator);
    };

    return m_receive_queue.fill<const char *>(start, generator, is_finish);
}

bool Context::process_next_command_blocking()
{
    {
        const std::lock_guard<std::mutex> lock(m_processing_mutex);
        std::string command; 
        if (!m_receive_queue.try_get(command))
        {
            return false;
        }

        m_handlers->PassThrough(command);

        std::shared_ptr<ProcessedBulk> bulk_ptr; 
        if (m_control_unit->ShouldClearStaticBulk())
        {
            auto bulk_ptr = make_shared<ProcessedBulk>(m_static_accumulator->MoveBulk(), m_control_unit->GetBulkStartTime());
        }
        else if(m_control_unit->GetState() == handlers::ControlUnit::ClearProcessedDynamic)
        {
            auto bulk_ptr = make_shared<ProcessedBulk>(m_dynamic_accumulator->MoveBulk(), m_control_unit->GetBulkStartTime());
        }
        else
        {
            return true;
        }
        
        s_logging_queue->put(bulk_ptr);
        s_output_queue->put(bulk_ptr);

        m_dynamic_accumulator->ClearBulk();
        m_control_unit->HandleEvent(handlers::ControlUnit::ClearedProcessed);

        return true;
    }
}

void Context::set_stopping_state()
{
    m_receive_queue.put(handlers::ControlCommand::EOF_COMMAND);
}

void Context::stop_background_workers()
{
    s_logging_queue->notify_stopping();
    s_output_queue->notify_stopping();

    s_logger_thread1.join();
    s_logger_thread2.join();
    s_output_thread.join();
}

}