#include <mutex>

#include "commands_parsing.h"
#include "context.h"
#include "handlers/accumulate_handler.h"
#include "handlers/control_unit_handler.h"
#include "handlers/handlers_chain.h"
#include "handlers/processing_handler.h"
#include "postprocessing/notifying_queue.h"
#include "postprocessing/processed_bulk.h"

// TODO: remove once debugged and tested
#include <iostream>

namespace async{

// TODO: remove once debugged and tested
static std::mutex g_console_mutex;

std::shared_ptr<postprocessing::notifying_queue<postprocessing::ProcessedBulk>> postprocessing_queue =
    std::make_shared<postprocessing::notifying_queue<postprocessing::ProcessedBulk>>();

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
            {
                const std::lock_guard<std::mutex> lock(g_console_mutex);
                std::cout << "bulk: ";
                for (const auto& cmd: m_accumulator->GetBulk())
                {
                    std::cout << cmd.Text << " ";
                }

                std::cout << std::endl;
            }

            postprocessing_queue->put(postprocessing::ProcessedBulk(m_accumulator->MoveBulk(), m_control_unit->GetBulkStartTime()));
            m_accumulator->ClearBulk();
            m_control_unit->HandleEvent(handlers::ControlUnit::ClearedProcessed);
        }
    }
}

}