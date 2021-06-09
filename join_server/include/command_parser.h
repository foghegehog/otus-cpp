#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "command.h"

#include <memory>
#include <string>
#include <queue>

class CommandParser
{
public:
    CommandParser( char separator, char terminator)
    :m_separator(separator), m_terminator(terminator)
    {}
    
    void ParseInput(const std::string& input_text);
    bool HasReadyCommands() const;
    Command GetReadyCommand();
private:
    Command ParseCommand(
        const std::string& full_text,
        std::string::size_type start,
        std::string::size_type text_end);

    char m_separator;
    char m_terminator;
    std::queue<Command> m_ready_commands;
    std::string m_unfinished_command;
};

#endif