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
    ProcessedBulk(){}

    ProcessedBulk(ProcessedBulk&& other) noexcept
        :m_commands(move(other.m_commands)), m_bulk_start_time(other.m_bulk_start_time)
    {
    }

    ProcessedBulk(std::vector<std::string>&& commands, time_t bulk_start_time)
        :m_commands(std::move(commands)),
        m_bulk_start_time(bulk_start_time)
    {
    }

    std::vector<std::string> m_commands; 
    time_t m_bulk_start_time;
};
}

#endif