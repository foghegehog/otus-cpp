#include "../../include/handlers/control_unit_handler.h"

namespace handlers{

void ControlUnitHandler::ExecuteCommand(const ExecutableCommand& command)
{
    UNUSED(command);
    m_control_unit->HandleEvent(ControlUnit::CommandAdded);
}

void ControlUnitHandler::HandleControlFlow(const ControlCommand& command)
{
    if (command.Text == "{")
    {
        m_control_unit->HandleEvent(ControlUnit::BlockOpened);
    }
    else if (command.Text == "}")
    {
        m_control_unit->HandleEvent(ControlUnit::BlockClosed);
    }
    else if (command.Text == "EOF")
    {
        m_control_unit->HandleEvent(ControlUnit::BlockOpened);
    }
    else
    { 
        throw std::invalid_argument("Unknown control flow command: " + command.Text);
    }
}

}