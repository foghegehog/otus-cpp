#pragma once

#include "accumulator.h"

#include <cstddef>
#include <ctime>
#include <functional>
#include <map>

namespace handlers{

/*!
 * Keeps track of current bulk state. 
 */
class ControlUnit
{
    public:
        enum State
        {
            Empty,
            GatheringStatic,
            BulkReady,
            ProcessUnfinished,
            GatheringDynamic,
            Discard,
            ClearProcessed,
            ClearProcessedUnfinished
        };

        enum Event
        {
            CommandAdded,
            BulkProcessed,
            ClearedProcessed,
            BlockOpened,
            BlockClosed,
            EndOfFile
        };

        ControlUnit(size_t static_bulk_size);
        void HandleEvent(Event evnt);
        bool ShouldProcessBulk() const;
        bool ShouldClearProcessedBulk() const;
        time_t GetBulkStartTime() const;
        
    private:
        State m_state = State::Empty;
        std::map<std::pair<State, Event>, std::function<void(void)>> m_state_machine; 
        size_t m_blocks_nesting = 0;
        size_t m_commands_count = 0;
        size_t m_static_bulk_size; 

        const time_t m_unitialized_time = (time_t)(-1); 
        time_t m_bulk_start_time = m_unitialized_time;

        void CreateTransition(State from, Event onEvent, std::function<void(void)> transition);
        void SaveBulkStartTime();
}; 

}