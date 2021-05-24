#ifndef TABLE_RECORD_H
#define TABLE_RECORD_H

#include <memory>
#include <string>

struct TableRecord
{
    TableRecord(int rec_id, std::string rec_name)
    :id(rec_id), name(rec_name)
    {}

    int id;
    std::string name;
};

struct JoinRecord
{
    JoinRecord(std::shared_ptr<TableRecord> left, std::shared_ptr<TableRecord> right)
    :m_left(left), m_right(right)
    {}

    std::shared_ptr<TableRecord> m_left;
    std::shared_ptr<TableRecord> m_right;
};

#endif