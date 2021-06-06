#ifndef SHARED_ACCUMULATOR_H
#define SHARED_ACCUMULATOR_H

#include <atomic>
#include <condition_variable>
#include <ctime>
#include <limits> 
#include <mutex>
#include <string>
#include <queue>
#include <set>
#include <vector>

namespace processing{

/*!
 * Accumulutes executable commands for shared bulks. 
 */
class SharedAccumulator
{
public:
    void SetBulkSize(size_t bulk_size);
    void StoreCommand(const std::string& command, int connection_id);
    std::vector<std::string> WaitBulk(time_t& start_time);
    void OnDisconnection(int connection_id);
private:
    // not to consider bulk ready until corect size is set
    size_t m_bulk_size = std::numeric_limits<int>::max();
    std::queue<std::string> m_commands;
    std::queue<time_t> m_bulks_starts;
    std::mutex m_mutex;
    std::condition_variable m_bulk_waiter; 
    std::set<int> m_connected_ids;
    std::atomic_bool m_all_disconnected;
};

}

#endif