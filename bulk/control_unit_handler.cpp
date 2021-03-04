#include "control_unit_handler.h"

void ControlUnitHandler::ExecuteCommand(const ExecutableCommand& command)
{
    UNUSED(command);
    m_contro_unit->HandleEvent(ControlUnit::CommandAdded);
}

void ControlUnitHandler::HandleControlFlow(const ControlCommand& command)
{
    if (command.Text == "{")
    {
        m_contro_unit->HandleEvent(ControlUnit::BlockOpened);
    }
    else if (command.Text == "}")
    {
        m_contro_unit->HandleEvent(ControlUnit::BlockClosed);
    }
    else if (command.Text == "EOF")
    {
        m_contro_unit->HandleEvent(ControlUnit::BlockOpened);
    }
    else
    { 
        throw invalid_argument("Unknown control flow command: " + command.Text);
    }
}