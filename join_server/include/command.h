#ifndef COMMAND_H
#define COMMAND_H

#include <ostream>
#include <string> 
#include <vector>

class Command
{
public:
    Command(std::string&& name)
    : m_name(move(name))
    {}

    void AddArgument(std::string&& argument);

    void Execute(std::ostream& outstream);
private:
    std::string m_name;
    std::vector<std::string> m_arguments;
};

#endif