#include <chrono>
#include <stdexcept>

#include "control_unit.h"

namespace handlers{

ControlUnit::ControlUnit(size_t static_bulk_size)
    :m_static_bulk_size(static_bulk_size)
{
    m_state = Empty;

    CreateTransition(Empty, CommandAdded, [this]()
    {
        SaveBulkStartTime();
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
        m_state = ClearProcessed; 
    });
    CreateTransition(ClearProcessed, ClearedProcessed, [this]()
    {
        m_state = Empty;
        m_bulk_start_time = m_unitialized_time; 
    });
    CreateTransition(ClearProcessedUnfinished, ClearedProcessed, [this]()
    {
        m_state = GatheringDynamic;
        m_bulk_start_time = m_unitialized_time; 
    });
    CreateTransition(Empty, BlockOpened, [this](){ m_state = GatheringDynamic; });
    CreateTransition(GatheringStatic, BlockOpened, [this](){ m_state = ProcessUnfinished; });
    CreateTransition(ProcessUnfinished, BulkProcessed, [this]()
    { 
        m_commands_count = 0;
        m_state = ClearProcessedUnfinished; 
    });
    CreateTransition(GatheringDynamic, CommandAdded, [this]()
    {
        if (m_bulk_start_time == m_unitialized_time)
        {
            SaveBulkStartTime();
        }
    });
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


bool ControlUnit::ShouldProcessBulk() const
{
    return (m_state == BulkReady) || (m_state == ProcessUnfinished);
}

bool ControlUnit::ShouldClearProcessedBulk() const
{
    return (m_state == ClearProcessed) || (m_state == ClearProcessedUnfinished);
}

time_t ControlUnit::GetBulkStartTime() const
{
    return m_bulk_start_time;
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

void ControlUnit::SaveBulkStartTime()
{
    auto now = std::chrono::system_clock::now(); 
    m_bulk_start_time = std::chrono::system_clock::to_time_t(now);
}

}