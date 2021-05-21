#include "../../include/processing/accumulator.h"

#include <chrono>
#include <ctime>

namespace processing{

void Accumulator::StoreCommand(const std::string& command)
{
    if (m_bulk.empty())
    {
        using namespace std::chrono;
        m_bulk_start_time = system_clock::to_time_t(system_clock::now());
    }

    m_bulk.emplace_back(command);
}

std::vector<std::string>&& Accumulator::MoveBulk()
{
    return std::move(m_bulk);
}

time_t Accumulator::GetLastBulkTime()
{
    return m_bulk_start_time;
}

}