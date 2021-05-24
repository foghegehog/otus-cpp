#ifndef COMMAND_H
#define COMMAND_H

#include <ostream>
#include <string> 
#include <vector>

struct Command
{
    Command(std::string&& name)
    : m_name(move(name))
    {}

    std::string m_name;
    std::vector<std::string> m_arguments;
};

#endif