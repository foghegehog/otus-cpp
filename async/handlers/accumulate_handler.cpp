#include "accumulate_handler.h"

namespace handlers{
void AccumulateHandler::ExecuteCommand(const ExecutableCommand& command, async::Context * context)
{
    context->m_accumulator.StoreCommand(command);
}

void AccumulateHandler::HandleControlFlow(const ControlCommand& command, async::Context * context)
{
    UNUSED(command);
    UNUSED(context);
}

}