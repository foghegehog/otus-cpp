#include "../../include/processing/accumulate_handler.h"

#include <stdexcept>

namespace processing{

void AccumulateHandler::HandleCommand(const std::string& command, int connection_id)
{
    if (command == "{")
    {
        m_control_unit.HandleEvent(ControlUnit::Event::BlockOpened);
    }
    else if (command == "}")
    {
        m_control_unit.HandleEvent(ControlUnit::Event::BlockClosed);
        if (m_control_unit.GetState() == ControlUnit::State::DynamicBulkReady)
        {
            m_processor->ProcessBulk(
                m_dynamic_accumulator.MoveBulk(),
                m_dynamic_accumulator.GetLastBulkTime());
            
            m_control_unit.HandleEvent(ControlUnit::Event::BulkProcessed);
        }
    }
    else
    {
        auto state = m_control_unit.GetState();
        if (state == ControlUnit::State::GatheringDynamic)
        { 
            m_dynamic_accumulator.StoreCommand(command);
            m_control_unit.HandleEvent(ControlUnit::Event::CommandAdded);
        }
        else if((state == ControlUnit::State::GatheringStatic) || (state == ControlUnit::State::Empty))
        {
            m_static_accumulator->StoreCommand(command, connection_id);
        }
        else
        {
            throw std::logic_error("Invalid control unit state!");
        }
    }
}

}