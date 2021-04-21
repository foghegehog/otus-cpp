#include "processing_handler.h"

namespace handlers{

void ProcessingHandler::ExecuteCommand(const ExecutableCommand& command, async::Context * context)
{
    UNUSED(command);
    ProcessBulkIfReady();
}

void ProcessingHandler::HandleControlFlow(const ControlCommand& command, async::Context * context)
{
    UNUSED(command);
    ProcessBulkIfReady();
}

void ProcessingHandler::ProcessBulkIfReady()
{
    if (m_contro_unit->ShouldProcessBulk())
    {
        auto bulk = m_accumulator->GetBulk();
        process(bulk);
        m_accumulator->ClearBulk();
        m_contro_unit->HandleEvent(ControlUnit::BulkProcessed);
    }
}

}