#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include <cstddef>
#include <functional>
#include <map>

namespace processing{

/*!
 * Keeps track of current bulks states. 
 */
class ControlUnit
{
    public:
        enum class State : uint8_t
        {
            Empty,
            GatheringStatic,
            GatheringDynamic,
            DynamicBulkReady
        };

        enum class Event : uint8_t
        {
            CommandAdded,
            BlockOpened,
            BlockClosed,
            BulkProcessed
        };

        ControlUnit();
        void HandleEvent(Event evnt);
        State GetState() const;
        
    private:
        State m_state = State::Empty;
        std::map<std::pair<State, Event>, std::function<void(void)>> m_state_machine; 
        size_t m_blocks_nesting = 0;

        void CreateTransition(State from, Event onEvent, std::function<void(void)> transition);

}; 

}

#endif