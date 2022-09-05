#pragma once

#include "accumulator.h"

#include <cstddef>
#include <functional>
#include <map>

/*!
 * Keeps track of current bulk state. 
 */
class ControlUnit
{
    public:
        enum class State : uint8_t
        {
            Empty,
            GatheringStatic,
            BulkReady,
            ProcessUnfinished,
            GatheringDynamic,
            Discard
        };

        enum class Event : uint8_t
        {
            CommandAdded,
            BulkProcessed,
            BlockOpened,
            BlockClosed,
            EndOfFile
        };

        ControlUnit(size_t static_bulk_size);
        void HandleEvent(Event evnt);
        bool ShouldProcessBulk() const;
        
    private:
        State m_state = State::Empty;
        std::map<std::pair<State, Event>, std::function<void(void)>> m_state_machine; 
        size_t m_blocks_nesting = 0;
        size_t m_commands_count = 0;
        size_t m_static_bulk_size; 

        void CreateTransition(State from, Event onEvent, std::function<void(void)> transition);
}; 