#include "../include/table.h"

#include <algorithm>
#include <mutex>

bool Table::Insert(int id, std::string name)
{
    {
        std::unique_lock lock(m_commands_mutex);
        auto result = m_records.emplace(std::make_shared<TableRecord>(id, name));
        return result.second;
    }
}

void Table::Truncate()
{
    {
        std::unique_lock lock(m_commands_mutex);
        m_records.clear();
    }
}

View Intersect(Table& left, Table& right)
{
    View result;
    RecordsCompare is_less;

    left.m_commands_mutex.lock_shared();
    right.m_commands_mutex.lock_shared();
        
    auto left_it = left.m_records.begin();
    auto right_it = right.m_records.begin();
    while(left_it != left.m_records.end() && right_it != right.m_records.end())
    {
        auto left_record = *left_it;
        auto right_record = *right_it;
        if (is_less(left_record, right_record))
        {
            ++left_it;
        }
        else
        {
            if (!is_less(right_record, left_record))
            {
                result.m_records.emplace_back(left_record, right_record);
                ++left_it;
            }
            ++right_it;
        }
    }

    left.m_commands_mutex.unlock_shared();
    right.m_commands_mutex.unlock_shared();

    return result;
}

