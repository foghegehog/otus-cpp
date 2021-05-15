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
    if (m_control_unit->ShouldProcessBulk())
    {
        auto bulk = m_accumulator->GetBulk();
        process(bulk);
        m_control_unit->HandleEvent(ControlUnit::BulkProcessed);
    }
}

}