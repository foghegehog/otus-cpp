#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H

#include <ctime>
#include <string>
#include <vector>

namespace processing{

/*!
 * Accumulutes executable commands. 
 */
class Accumulator
{
public:
    void StoreCommand(const std::string& command);
    std::vector<std::string>&& MoveBulk();
    std::time_t GetLastBulkTime();
private:
    std::vector<std::string> m_bulk; 
    const time_t m_unitialized_time = (time_t)(-1); 
    time_t m_bulk_start_time = m_unitialized_time;
};

}

#endif