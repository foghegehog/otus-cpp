#ifndef TABLE_H
#define TABLE_H

#include "table_record.h"

#include <memory>
#include <shared_mutex>
#include <set>
#include <string>
#include <vector>

struct RecordsCompare
{
    bool operator()(
        const std::shared_ptr<TableRecord>& left,
        const std::shared_ptr<TableRecord>& right) const
    {
        return left->id < right->id;
    }
};


struct View
{
    std::vector<JoinRecord> m_records;    
};

class Table
{
public:
    bool Insert(int id, std::string name); 
    void Truncate(); 
    friend View Intersect(Table& left, Table& right);
    friend View SymmetricDifference(Table& left, Table& right);
private:
    std::set<std::shared_ptr<TableRecord>, RecordsCompare> m_records;
    std::shared_mutex m_commands_mutex;
};
#endif