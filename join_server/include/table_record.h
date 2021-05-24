#ifndef TABLE_RECORD_H
#define TABLE_RECORD_H

#include <string>

struct TableRecord
{
    TableRecord(int rec_id, std::string rec_name)
    :id(rec_id), name(rec_name)
    {}

    int id;
    std::string name;
};

#endif