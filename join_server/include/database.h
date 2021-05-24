#ifndef DATABASE_H
#define DATABASE_H

#include "command.h"
#include "table.h"

#include <functional>
#include <ostream>
#include <map>
#include <mutex>

class Database
{
public:
    Database();
    void Interpret(Command command, std::ostream& outstream);
    
private:
    void InterpretInsert(const std::vector<std::string>& args, std::ostream& outstream);
    void InterpretTruncate(const std::vector<std::string>& args, std::ostream& outstream);
    void InterpretIntersection(const std::vector<std::string>& args, std::ostream& outstream);

    bool FindTable(const std::string& table_name, Table*& table);
    void WriteSuccess(std::ostream& outstream);
    void WriteJoinRecord(const JoinRecord& join, std::ostream& outstream);

    Table A;
    Table B;
    std::map<std::string, Table*> m_tables
    {
        std::make_pair(std::string("A"), &A),
        std::make_pair(std::string("B"), &B)
    }; 

    std::map<
        std::string,
        void (Database::*)(const std::vector<std::string>&, std::ostream&)> m_interpreters;

};

#endif