#include <iostream>

#include "../../include/handlers/processing_handler.h"

namespace handlers{

void ProcessingHandler::ExecuteCommand(const ExecutableCommand& command)
{
    UNUSED(command);
    ProcessBulkIfReady();
}

void ProcessingHandler::HandleControlFlow(const ControlCommand& command)
{
    UNUSED(command);
    ProcessBulkIfReady();
}

void ProcessingHandler::ProcessBulkIfReady()
{
    {
        std::lock_guard<std::mutex> lock(m_shared_mutex);
        if (m_control_unit->ShouldProcessStaticBulk())
        {
            auto bulk = m_shared_accumulator->GetBulk();
            process(bulk);
        }
        else if(m_control_unit->GetState() == ControlUnit::DynamicBulkReady)
        {
            auto bulk = m_dynamic_accumulator->GetBulk();
            process(bulk);
        }
        else
        {
            return;
        }
        
        m_control_unit->HandleEvent(ControlUnit::BulkProcessed);
    }
}

}