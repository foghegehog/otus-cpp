#include <chrono>
#include <stdexcept>

#include "../../include/processing/control_unit.h"

namespace processing{

ControlUnit::ControlUnit()
{
    CreateTransition(State::Empty, Event::CommandAdded, [this]()
    {
        m_state = State::GatheringStatic;
    });
    
    CreateTransition(State::GatheringStatic, Event::CommandAdded, []()
    {
    });

    CreateTransition(State::Empty, Event::BlockOpened, [this]()
    {
        m_state = State::GatheringDynamic; 
    });

    CreateTransition(State::GatheringStatic, Event::BlockOpened, [this]()
    {
        m_state = State::GatheringDynamic; 
    });

    CreateTransition(State::GatheringDynamic, Event::CommandAdded, [this]()
    {
    });

    CreateTransition(State::GatheringDynamic, Event::BlockOpened, [this]()
    {
        m_blocks_nesting++;
    });

    CreateTransition(State::GatheringDynamic, Event::BlockClosed, [this]()
    {
        if (m_blocks_nesting == 0)
        {
            m_state = State::DynamicBulkReady;
        }
        else
        {
            m_blocks_nesting--;
        }
    });
}

void ControlUnit::HandleEvent(Event evnt)
{
    auto transition_key = std::make_pair(m_state, evnt);
    auto transition = m_state_machine.find(transition_key);
    if (transition != m_state_machine.end()) 
    {
        auto action = transition->second;
        action();
    }
    else
    {
        throw std::logic_error("Invalid event for current state!");
    }
}

void ControlUnit::CreateTransition(State from, Event onEvent, std::function<void(void)> transition)
{
    auto key = std::make_pair(from, onEvent);
    m_state_machine[key] = transition;
}

ControlUnit::State ControlUnit::GetState() const
{
    return m_state;
}

}