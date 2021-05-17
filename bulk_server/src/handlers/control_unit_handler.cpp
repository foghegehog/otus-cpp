#include "../../include/handlers/control_unit_handler.h"

namespace handlers{

void ControlUnitHandler::ExecuteCommand(const ExecutableCommand& command)
{
    UNUSED(command);
    {
        std::lock_guard<std::mutex> lock(m_shared_mutex);
        if(m_shared_accumulator->GetCommandsStoredCount() == m_control_unit->GetStaticBulkSize())
        {
            m_control_unit->HandleEvent(ControlUnit::StaticBulkSizeReached);
        }
        else
        {
            m_control_unit->HandleEvent(ControlUnit::CommandAdded);
        }
    }
}

void ControlUnitHandler::HandleControlFlow(const ControlCommand& command)
{
    {
        std::lock_guard<std::mutex> lock(m_shared_mutex);
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

}