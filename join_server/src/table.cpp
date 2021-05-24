#include "../include/table.h"

bool Table::Insert(int id, std::string name)
{
    m_commands_mutex.lock();
    auto result = m_records.emplace(std::make_shared<TableRecord>(id, name));
    m_commands_mutex.unlock();
    return result.second;
}

