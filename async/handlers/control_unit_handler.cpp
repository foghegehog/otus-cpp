#include "control_unit_handler.h"

namespace handlers{

void ControlUnitHandler::ExecuteCommand(const ExecutableCommand& command, async::Context * context)
{
    UNUSED(command);
    context->m_control_unit.HandleEvent(ControlUnit::CommandAdded);
}

void ControlUnitHandler::HandleControlFlow(const ControlCommand& command, async::Context * context)
{
    if (command.Text == "{")
    {
        context->m_control_unit.HandleEvent(ControlUnit::BlockOpened);
    }
    else if (command.Text == "}")
    {
        context->m_control_unit.HandleEvent(ControlUnit::BlockClosed);
    }
    else if (command.Text == "EOF")
    {
        context->m_control_unit.HandleEvent(ControlUnit::BlockOpened);
    }
    else
    { 
        throw std::invalid_argument("Unknown control flow command: " + command.Text);
    }
}

}