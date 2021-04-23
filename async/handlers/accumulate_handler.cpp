#include "accumulate_handler.h"

namespace handlers{
void AccumulateHandler::ExecuteCommand(const ExecutableCommand& command)
{
    m_accumulator->StoreCommand(command);
}

void AccumulateHandler::HandleControlFlow(const ControlCommand& command)
{
    UNUSED(command);
}

}