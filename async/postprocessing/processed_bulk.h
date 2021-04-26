#ifndef PROCESSED_BULK_H
#define PROCESSED_BULK_H

#include "../handlers/command.h"

#include <atomic>
#include <chrono>
#include <vector>

namespace postprocessing{
struct ProcessedBulk
{
public:
    ProcessedBulk(std::vector<handlers::ExecutableCommand>&& commands, time_t bulk_start_time);

    std::vector<handlers::ExecutableCommand> m_commands; 
    time_t m_bulk_start_time;
    std::atomic_ushort m_postprocessings_count;
};
}

#endif