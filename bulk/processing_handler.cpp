#include "processing_handler.h"

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
    if ((m_contro_unit->GetState() == ControlUnit::BulkReady)
        || (m_contro_unit->GetState() == ControlUnit::ProcessUnfinished))
    {
        auto bulk = m_accumulator->GetBulk();
        process(bulk);
        m_accumulator->ClearBulk();
        m_contro_unit->HandleEvent(ControlUnit::BulkProcessed);
    }
}