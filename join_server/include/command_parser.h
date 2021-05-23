#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "command.h"

#include <memory>
#include <string>

class CommandParser
{
public:
    CommandParser( char separator, char terminator)
    :m_separator(separator), m_terminator(terminator)
    {}
    
    Command ParseCommand(const std::string& command_text);
private:
    char m_separator;
    char m_terminator;
};

#endif