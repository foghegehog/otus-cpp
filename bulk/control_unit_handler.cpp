#include "control_unit_handler.h"

void ControlUnitHandler::ExecuteCommand(const ExecutableCommand& command)
{
    UNUSED(command);
    m_contro_unit->HandleEvent(ControlUnit::Event::CommandAdded);
}

void ControlUnitHandler::HandleControlFlow(const ControlCommand& command)
{
    if (command.Text == "{")
    {
        m_contro_unit->HandleEvent(ControlUnit::Event::BlockOpened);
    }
    else if (command.Text == "}")
    {
        m_contro_unit->HandleEvent(ControlUnit::Event::BlockClosed);
    }
    else if (command.Text == "EOF")
    {
        m_contro_unit->HandleEvent(ControlUnit::Event::BlockOpened);
    }
    else
    { 
        throw std::invalid_argument("Unknown control flow command: " + command.Text);
    }
}