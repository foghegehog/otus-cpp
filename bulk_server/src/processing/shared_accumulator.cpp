#include "../../include/processing/shared_accumulator.h"

namespace processing{


void SharedAccumulator::SetBulkSize(size_t bulk_size)
{
    m_bulk_size = bulk_size;
}

void SharedAccumulator::StoreCommand(const std::string& command, int connection_id)
{
    using namespace std;

    {
        lock_guard<std::mutex> lock(m_mutex);
        if (m_commands.size() % m_bulk_size == 0)
        {
            using namespace std::chrono;
            auto now = system_clock::to_time_t(system_clock::now());
            m_bulks_starts.push(now);
        }

        m_commands.push(command);
        m_connected_ids.emplace(connection_id);
    }

    m_bulk_waiter.notify_one();
}

std::vector<std::string> SharedAccumulator::WaitBulk(time_t& start_time)
{
    using namespace std;

    vector<string> bulk;

    {
        unique_lock<std::mutex> lock(m_mutex);
        m_bulk_waiter.wait(lock, [this]
        {
            return (m_commands.size() >= m_bulk_size) || m_all_disconnected.load(); 
        });

        auto bulk_size = m_bulk_size < m_commands.size() ? m_bulk_size : m_commands.size();
        for (size_t c = 0; c < bulk_size; c++)
        {
            bulk.emplace_back(move(m_commands.front()));
            m_commands.pop();
        }

        start_time = m_bulks_starts.front();
        m_bulks_starts.pop();
        m_all_disconnected.store(false);
    }

    return bulk;

}

void SharedAccumulator::OnDisconnection(int connection_id)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_connected_ids.erase(connection_id);
        if (m_connected_ids.empty())
        {
            m_all_disconnected.store(true);
        }
    }

    m_bulk_waiter.notify_one();
}


}