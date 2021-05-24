#ifndef TABLE_H
#define TABLE_H

#include "table_record.h"

#include <memory>
#include <shared_mutex>
#include <set>
#include <string>

struct RecordsCompare
{
    bool operator()(
        const std::shared_ptr<TableRecord>& left,
        const std::shared_ptr<TableRecord>& right)
    {
        return left->id < right->id;
    }
};

class Table
{
public:
    bool Insert(int id, std::string name); 
private:
    std::set<std::shared_ptr<TableRecord>, RecordsCompare> m_records;
    std::shared_mutex m_commands_mutex;

};
#endif