#ifndef THREADSAFE_ACCUMULATOR_H
#define THREADSAFE_ACCUMULATOR_H

#include "accumulator.h"
#include <mutex>

namespace handlers{

class ThreadSafeAccumulator : public Accumulator
{
public:
    void StoreCommand(const ExecutableCommand& command) override;
    size_t GetCommandsStoredCount() override;
    std::vector<ExecutableCommand>&& MoveBulk() override;
    void ClearBulk() override;
private:
    std::mutex m_mutex;
};

}

#endif