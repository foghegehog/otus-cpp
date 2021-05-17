#include "../../include/handlers/accumulate_handler.h"

namespace handlers{
void AccumulateHandler::ExecuteCommand(const ExecutableCommand& command)
{
    {
        std::lock_guard<std::mutex> lock(m_shared_mutex);
        auto state = m_control_unit->GetState();
        if ((state == ControlUnit::State::Empty) || (state == ControlUnit::State::GatheringStatic))
        {
            m_shared_accumulator->StoreCommand(command);
        }
        else if (state == ControlUnit::State::GatheringDynamic)
        {
            m_dynamic_accumulator->StoreCommand(command);
        }
    }
}

void AccumulateHandler::HandleControlFlow(const ControlCommand& command)
{
    UNUSED(command);
}

}