#pragma once

#include "accumulator.h"

#include <cstddef>
#include <functional>
#include <map>

using namespace std;

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
            Discard
        };

        enum Event
        {
            CommandAdded,
            BulkProcessed,
            BlockOpened,
            BlockClosed,
            EndOfFile
        };

        ControlUnit(size_t static_bulk_size);
        void HandleEvent(Event evnt);
        State GetState() const;
        
    private:
        State m_state = State::Empty;
        map<pair<State, Event>, function<void(void)>> m_state_machine; 
        size_t m_blocks_nesting = 0;
        size_t m_commands_count = 0;
        size_t m_static_bulk_size; 

        void CreateTransition(State from, Event onEvent, function<void(void)> transition);
}; 