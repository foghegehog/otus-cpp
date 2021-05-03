#include <stdexcept>

#include "control_unit.h"

ControlUnit::ControlUnit(size_t static_bulk_size)
    :m_static_bulk_size(static_bulk_size)
{
    m_state = State::Empty;

    CreateTransition(State::Empty, Event::CommandAdded, [this]()
    {
        m_commands_count++;
        m_state = m_commands_count < m_static_bulk_size ? State::GatheringStatic : State::BulkReady; 
    });
    CreateTransition(State::GatheringStatic, Event::CommandAdded, [this]()
    {
        m_commands_count++; 
        if (m_commands_count == m_static_bulk_size)
        {
            m_state = State::BulkReady;
        }
    });
    CreateTransition(State::GatheringStatic, Event::EndOfFile, [this](){ m_state = State::BulkReady; });
    CreateTransition(State::BulkReady, Event::BulkProcessed, [this]()
    {
        m_commands_count = 0;
        m_state = State::Empty; 
    });
    CreateTransition(State::Empty, Event::BlockOpened, [this](){ m_state = State::GatheringDynamic; });
    CreateTransition(State::GatheringStatic, Event::BlockOpened, [this](){ m_state = State::ProcessUnfinished; });
    CreateTransition(State::ProcessUnfinished, Event::BulkProcessed, [this]()
    { 
        m_commands_count = 0;
        m_state = State::GatheringDynamic; 
    });
    CreateTransition(State::GatheringDynamic, Event::CommandAdded, [](){});
    CreateTransition(State::GatheringDynamic, Event::BlockOpened, [this]()
    {
        m_blocks_nesting++;
    });
    CreateTransition(State::GatheringDynamic, Event::BlockClosed, [this]()
    {
        if (m_blocks_nesting == 0)
        {
            m_state = State::BulkReady;
        }
        else
        {
            m_blocks_nesting--;
        }
    });
    CreateTransition(State::GatheringDynamic, Event::EndOfFile, [this](){ m_state = State::Discard; });
}


void ControlUnit::CreateTransition(State from, Event onEvent, std::function<void(void)> transition)
{
    auto key = std::make_pair(from, onEvent);
    m_state_machine[key] = transition;
}

bool ControlUnit::ShouldProcessBulk() const
{
    return (m_state == State::BulkReady) || (m_state == State::ProcessUnfinished);
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