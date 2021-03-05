#include <stdexcept>

#include "control_unit.h"

ControlUnit::ControlUnit(size_t static_bulk_size)
    :m_static_bulk_size(static_bulk_size)
{
    m_state = Empty;

    CreateTransition(Empty, CommandAdded, [this]()
    {
        m_commands_count++;
        m_state = m_commands_count < m_static_bulk_size ? GatheringStatic : BulkReady; 
    });
    CreateTransition(GatheringStatic, CommandAdded, [this]()
    {
        m_commands_count++; 
        if (m_commands_count == m_static_bulk_size)
        {
            m_state = BulkReady;
        }
    });
    CreateTransition(GatheringStatic, EndOfFile, [this](){ m_state = BulkReady; });
    CreateTransition(BulkReady, BulkProcessed, [this]()
    {
        m_commands_count = 0;
        m_state = Empty; 
    });
    CreateTransition(Empty, BlockOpened, [this](){ m_state = GatheringDynamic; });
    CreateTransition(GatheringStatic, BlockOpened, [this](){ m_state = ProcessUnfinished; });
    CreateTransition(ProcessUnfinished, BulkProcessed, [this]()
    { 
        m_commands_count = 0;
        m_state = GatheringDynamic; 
    });
    CreateTransition(GatheringDynamic, CommandAdded, [](){});
    CreateTransition(GatheringDynamic, BlockOpened, [this]()
    {
        m_blocks_nesting++;
    });
    CreateTransition(GatheringDynamic, BlockClosed, [this]()
    {
        if (m_blocks_nesting == 0)
        {
            m_state = BulkReady;
        }
        else
        {
            m_blocks_nesting--;
        }
    });
    CreateTransition(GatheringDynamic, EndOfFile, [this](){ m_state = Discard; });
}


void ControlUnit::CreateTransition(State from, Event onEvent, function<void(void)> transition)
{
    auto key = make_pair(from, onEvent);
    m_state_machine[key] = transition;
}

bool ControlUnit::ShouldProcessBulk() const
{
    return (m_state == BulkReady) || (m_state == ProcessUnfinished);
}

void ControlUnit::HandleEvent(Event evnt)
{
    auto transition_key = make_pair(m_state, evnt);
    auto transition = m_state_machine.find(transition_key);
    if (transition != m_state_machine.end()) 
    {
        auto action = transition->second;
        action();
    }
    else
    {
        throw logic_error("Invalid event for current state!");
    }
}