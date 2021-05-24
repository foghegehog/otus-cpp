#include "../include/database.h"

Database::Database()
{
    m_interpreters.emplace("INSERT", &Database::InterpretInsert);
    m_interpreters.emplace("TRUNCATE", &Database::InterpretTruncate);
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

void Database::InterpretInsert(const std::vector<std::string>& args, std::ostream& outstream)
{
    if (args.size() != 3)
    {
        throw std::invalid_argument("wrong args: 'table id name' are needed");
    }

    Table * table;
    if (!Database::FindTable(args[0], table))
    {
        throw std::invalid_argument("no table " + args[0]);
    }

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

    WriteSuccess(outstream);
}

void Database::InterpretTruncate(const std::vector<std::string>& args, std::ostream& outstream)
{
    if (args.size() != 1)
    {
        throw std::invalid_argument("wrong args: table_name is needed");
    }

    Table * table;
    if (!Database::FindTable(args[0], table))
    {
        throw std::invalid_argument("no table " + args[0]);
    }

    table->Truncate();
    WriteSuccess(outstream);
}

bool Database::FindTable(const std::string& table_name, Table*& table)
{
    auto it_table = m_tables.find(table_name);
    if (it_table == m_tables.end())
    {
        return false;
    }

    table = it_table->second;
    return true;
}

void Database::WriteSuccess(std::ostream& outstream)
{
    outstream << "OK" << std::endl;
}
