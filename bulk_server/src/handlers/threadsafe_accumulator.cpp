#include "../../include/handlers/threadsafe_accumulator.h"

namespace handlers{

void ThreadSafeAccumulator::StoreCommand(const ExecutableCommand& command)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        Accumulator::StoreCommand(command); 
    }
}

size_t ThreadSafeAccumulator::GetCommandsStoredCount() 
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return Accumulator::GetCommandsStoredCount();
    }
}

std::vector<ExecutableCommand>&& ThreadSafeAccumulator::MoveBulk()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return Accumulator::MoveBulk();
    }
}

void ThreadSafeAccumulator::ClearBulk()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        ThreadSafeAccumulator::ClearBulk();
    }
}
}