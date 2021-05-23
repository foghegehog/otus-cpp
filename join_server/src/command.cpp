#include "../include/command.h"

void Command::AddArgument(std::string&& arg)
{
    m_arguments.push_back(move(arg));
}

void Command::Execute(std::ostream& outstream)
{
    outstream << "Command: " << m_name << ", arguments: ";
    for(const auto& a: m_arguments)
    {
        outstream << a << ", ";
    }
    outstream << std::endl;
}