#include <mutex>
#include <thread>

#include "commands_parsing.h"
#include "context.h"
#include "handlers/accumulate_handler.h"
#include "handlers/control_unit_handler.h"
#include "handlers/handlers_chain.h"
#include "handlers/processing_handler.h"
#include "postprocessing/logging_handler.h"
#include "postprocessing/output_handler.h"
#include "postprocessing/notifying_queue.h"
#include "postprocessing/postprocessing.h"
#include "postprocessing/processed_bulk.h"

// TODO: remove once debugged and tested
#include <iostream>

namespace async{

// TODO: remove once debugged and tested
static std::mutex g_console_mutex;

using namespace postprocessing;
using namespace std;

static shared_ptr<notifying_queue<shared_ptr<ProcessedBulk>>> logging_queue = make_shared<notifying_queue<shared_ptr<ProcessedBulk>>>();
static shared_ptr<notifying_queue<shared_ptr<ProcessedBulk>>> output_queue = make_shared<notifying_queue<shared_ptr<ProcessedBulk>>>();

void start_threads()
{
    thread output_thread(&Postprocessing::Run, Postprocessing(make_unique<OutputHandler>(), output_queue));
    output_thread.detach();

    thread file1_thread(&Postprocessing::Run, Postprocessing(make_unique<LoggingHandler>("file1"), logging_queue));
    file1_thread.detach();
    thread file2_thread(&Postprocessing::Run, Postprocessing(make_unique<LoggingHandler>("file2"), logging_queue));
    file2_thread.detach();
}

Context::Context(size_t bulk_size)
{
    using namespace handlers;

    m_accumulator = std::make_shared<handlers::Accumulator>(); 
    m_control_unit = std::make_shared<handlers::ControlUnit>(bulk_size);
    
    auto processing_handler_factory = [this]()
    {
        return std::make_unique<ProcessingHandler>(m_control_unit, m_accumulator, [](){ return nullptr; }); 
    };

    auto control_handler_factory = [this, processing_handler_factory]()
    {
        return std::make_unique<ControlUnitHandler>(m_control_unit, processing_handler_factory); 
    };

    auto accumulator_handler_factory = [this, control_handler_factory]()
    {
        return std::make_unique<AccumulateHandler>(m_accumulator, control_handler_factory); 
    };

    m_handlers = std::make_unique<HandlersChain>(accumulator_handler_factory);
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

void Context::process_next_command_blocking()
{
    {
        const std::lock_guard<std::mutex> lock(m_processing_mutex);
        auto next_command = m_receive_queue.get();
        m_handlers->PassThrough(next_command);

        if (m_control_unit->ShouldClearProcessedBulk())
        {
            auto bulk_ptr = make_shared<ProcessedBulk>(m_accumulator->MoveBulk(), m_control_unit->GetBulkStartTime());
            logging_queue->put(bulk_ptr);
            output_queue->put(bulk_ptr);

            m_accumulator->ClearBulk();
            m_control_unit->HandleEvent(handlers::ControlUnit::ClearedProcessed);
        }
    }
}

}