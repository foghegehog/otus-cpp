#include <chrono>
#include <stdexcept>

#include "../../include/handlers/control_unit.h"

namespace handlers{

ControlUnit::ControlUnit(size_t static_bulk_size)
    :m_static_bulk_size(static_bulk_size)
{
    m_state = Empty;

    CreateTransition(Empty, CommandAdded, [this]()
    {
        SaveBulkStartTime();
        m_state = GatheringStatic;
    });
    CreateTransition(GatheringStatic, CommandAdded, [](){});
    CreateTransition(GatheringStatic, StaticBulkSizeReached, [this]()
    {
            m_state = StaticBulkReady;
    });
    CreateTransition(GatheringStatic, EndOfFile, [this](){ m_state = StaticBulkReady; });
    CreateTransition(StaticBulkReady, BulkProcessed, [this]()
    {
        m_state = ClearProcessedStatic; 
    });
    CreateTransition(ClearProcessedStatic, ClearedProcessed, [this]()
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
            m_state = DynamicBulkReady;
        }
        else
        {
            m_blocks_nesting--;
        }
    });
    CreateTransition(GatheringDynamic, BulkProcessed, [this]()
    {
        m_state = ClearProcessedDynamic; 
    });
    CreateTransition(ClearProcessedDynamic, ClearedProcessed, [this]()
    {
        m_state = Empty;
        m_bulk_start_time = m_unitialized_time; 
    });
    CreateTransition(GatheringDynamic, EndOfFile, [this](){ m_state = Discard; });
}


bool ControlUnit::ShouldProcessStaticBulk() const
{
    return (m_state == StaticBulkReady) || (m_state == ProcessUnfinished);
}

bool ControlUnit::ShouldClearStaticBulk() const
{
    return (m_state == ClearProcessedStatic) || (m_state == ClearProcessedUnfinished);
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

ControlUnit::State ControlUnit::GetState() const
{
    return m_state;
}

size_t ControlUnit::GetStaticBulkSize() const
{
    return m_static_bulk_size;
}

}