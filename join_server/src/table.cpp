#include "../include/table.h"

#include <algorithm>
#include <mutex>
#include <shared_mutex>

/*!
 * Inserts new record into the table. 
 * Due to usage of std::set as the underlying structure, the complexity of the operation is O(log(n)).    
 */
bool Table::Insert(int id, std::string name)
{
    {
        std::unique_lock<std::shared_mutex> lock(m_commands_mutex);
        auto result = m_records.emplace(std::make_shared<TableRecord>(id, name));
        return result.second;
    }
}

/*!
 * Erases all the records in the table. 
 * Due to usage of std::set as the underlying structure, the complexity of the operation is O(n).  
 */
void Table::Truncate()
{
    {
        std::unique_lock<std::shared_mutex> lock(m_commands_mutex);
        m_records.clear();
    }
}

/*!
 * Intersects content of two tables. 
 * The implementation is similar to std::set_intersection (https://en.cppreference.com/w/cpp/algorithm/set_intersection),
 * as the underlying structure (std::set) is sorted upon insertion.  
 * Thus, the complexity is at most 2·(N1+N2)-1 comparisons, 
 * where N1 is a distance (numper of hops) between the first and the last elements in the firsts table,
 * N2 - a distance between the first and the last elements in the second table.     
 */
View Intersect(const Table& left, const Table& right)
{
    View result;
    RecordsCompare is_less;

    {
        std::shared_lock<std::shared_mutex> left_lock(left.m_commands_mutex, std::defer_lock);
        std::shared_lock<std::shared_mutex> right_lock(right.m_commands_mutex, std::defer_lock);
        std::lock(left_lock, right_lock);        

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

        return result;
    }
}

/*!
 * Takes symmetric difference of the content of two tables. 
 * The implementation is similar to std::set_symmetric_difference (https://en.cppreference.com/w/cpp/algorithm/set_symmetric_difference),
 * as the underlying structure (std::set) is sorted upon insertion.  
 * Thus, the complexity is at most 2·(N1+N2)-1 comparisons, 
 * where N1 is a distance (numper of hops) between the first and the last elements in the firsts table,
 * N2 - a distance between the first and the last elements in the second table.     
 */
View SymmetricDifference(const Table& left, const Table& right)
{
    View result;
    RecordsCompare is_less;

    {
        std::shared_lock<std::shared_mutex> left_lock(left.m_commands_mutex, std::defer_lock);
        std::shared_lock<std::shared_mutex> right_lock(right.m_commands_mutex, std::defer_lock);
        std::lock(left_lock, right_lock);        
        

        auto left_it = left.m_records.begin();
        auto right_it = right.m_records.begin();
        while(left_it != left.m_records.end())
        {
            if(right_it == right.m_records.end())
            {
                while(left_it != left.m_records.end())
                {
                    auto left_record = *left_it;
                    auto empty_record = std::make_shared<TableRecord>(left_record->id, "");
                    result.m_records.emplace_back(left_record, empty_record);
                    ++left_it;
                }

                break;
            }

            auto left_record = *left_it;
            auto right_record = *right_it;
            if(is_less(left_record, right_record))
            {
                auto empty_record = std::make_shared<TableRecord>(left_record->id, "");
                result.m_records.emplace_back(left_record, empty_record);
                ++left_it;
            }
            else
            {
                if(is_less(right_record, left_record))
                {
                    auto empty_record = std::make_shared<TableRecord>(right_record->id, "");
                    result.m_records.emplace_back(empty_record, right_record);
                }
                else
                {
                    ++left_it;
                }

                ++right_it;
            }
        }

        while(right_it != right.m_records.end())
        {
            auto right_record = *right_it;
            auto empty_record = std::make_shared<TableRecord>(right_record->id, "");
            result.m_records.emplace_back(empty_record, right_record);
            ++right_it;
        }

        return result;

    }
}

