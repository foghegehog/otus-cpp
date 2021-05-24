#include "../include/database.h"

Database::Database()
{
    m_interpreters.emplace("INSERT", &Database::InterpretInsert);
}

void Database::InterpretInsert(const std::vector<std::string>& args, std::ostream& outstream)
{
    if (args.size() != 3)
    {
        throw std::invalid_argument("wrong args: 'table id name' are needed" + args[1]);
    }

    auto it_table = m_tables.find(args[0]);
    if (it_table == m_tables.end())
    {
        throw std::invalid_argument("no table " + args[0]);
    }

    auto table = it_table->second;
    int id;
    try
    {
        id = std::stoi(args[1]);
    }
    catch(const std::exception& e)
    {
        throw std::invalid_argument("wrong id format: " + args[1]);
    }

    if (!table->Insert(id, args[2]))
    {
        throw std::invalid_argument("duplicate " + args[1]);
    }

    outstream << "OK" << std::endl;
    
}

void Database::Interpret(Command command, std::ostream& outstream)
{
    auto it_interpreter = m_interpreters.find(command.m_name);
    if (it_interpreter == m_interpreters.end())
    {
        throw std::invalid_argument("unknown command " + command.m_name);
    }

    auto interpreter = it_interpreter->second;
    std::invoke(interpreter, this, command.m_arguments, outstream);
}

void Database::Insert(std::string table, int id, std::string name)
{
    m_tables[table]->Insert(id, name);
}