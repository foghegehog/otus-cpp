#include <iostream>

#include "processing_handler.h"

namespace handlers{

void ProcessingHandler::ExecuteCommand(const ExecutableCommand& command, async::Context * context)
{
    UNUSED(command);
    ProcessBulkIfReady(context);
}

void ProcessingHandler::HandleControlFlow(const ControlCommand& command, async::Context * context)
{
    UNUSED(command);
    ProcessBulkIfReady(context);
}

void ProcessingHandler::ProcessBulkIfReady(async::Context * context)
{
    if (context->m_control_unit.ShouldProcessBulk())
    {
        auto bulk = context->m_accumulator.GetBulk();
        process(bulk);
        std::cout << "Bulk: ";
        for (const auto& cmd: bulk)
        {
            std::cout << cmd.Text << std::endl;
        } 
        context->m_accumulator.ClearBulk();
        context->m_control_unit.HandleEvent(ControlUnit::BulkProcessed);
    }
}

}