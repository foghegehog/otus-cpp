#include <atomic>
#include <mutex>
#include <thread>

#include "../include/commands_parsing.h"
#include "../include/context.h"


namespace async{

using namespace postprocessing;
using namespace std;

thread Context::s_static_processor_thread = thread([]()
{  
    time_t bulk_time; 
    while(true)
    {
        s_static_processor->ProcessBulk(
            s_static_accumulator->WaitBulk(bulk_time),
            bulk_time);
    }
});

thread Context::s_logger_thread1 = thread(
            &Postprocessing::Run,
            Postprocessing(make_unique<LoggingHandler>("file1"), s_logging_queue));
thread Context::s_logger_thread2 = thread(
            &Postprocessing::Run,
            Postprocessing(make_unique<LoggingHandler>("file2"), s_logging_queue));

thread Context::s_output_thread = thread(
            &Postprocessing::Run, Postprocessing(make_unique<OutputHandler>(), s_output_queue));

/*static std::shared_ptr<handlers::Accumulator> get_shared_accumulator()
{
    static std::shared_ptr<handlers::Accumulator> instance = std::make_shared<handlers::Accumulator>();                               
    return instance;
}

static std::mutex& get_shared_accumulator_mutex()
{
    static std::mutex instance;                               
    return instance;
}*/


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

        m_accumulate_handler->HandleCommand(command);
        /*m_handlers->PassThrough(command);

        {
            std::lock_guard<std::mutex> lock(m_shared_accumulator_mutex);
            std::shared_ptr<ProcessedBulk> bulk_ptr; 
            if (m_control_unit->ShouldClearStaticBulk())
            {
                bulk_ptr = make_shared<ProcessedBulk>(m_shared_accumulator->MoveBulk(), m_control_unit->GetBulkStartTime());
            }
            else if(m_control_unit->GetState() == handlers::ControlUnit::ClearProcessedDynamic)
            {
                bulk_ptr = make_shared<ProcessedBulk>(m_dynamic_accumulator->MoveBulk(), m_control_unit->GetBulkStartTime());
            }
            else
            {
                return true;
            }
            
            s_logging_queue->put(bulk_ptr);
            s_output_queue->put(bulk_ptr);

            m_control_unit->HandleEvent(handlers::ControlUnit::ClearedProcessed);
        }*/

        return true;
    }
}
void Context::set_bulk_size(size_t bulk_size)
{
    s_static_accumulator->SetBulkSize(bulk_size);
}

void Context::set_stopping_state()
{
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