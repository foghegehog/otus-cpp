#include "../include/command.h"
#include "../include/command_parser.h"

#include <string>

void CommandParser::ParseInput(const std::string& input_text)
{
    using namespace std;

    if (input_text.empty())
    {
        return;
    }

    if (!m_unfinished_command.empty())
    {
        m_unfinished_command += input_text;
    }

    const string& parse_str = m_unfinished_command.empty() ? input_text : m_unfinished_command;

    string::size_type start = 0;
    string::size_type terminator_pos;
    while(start < parse_str.size())
    {
        terminator_pos = parse_str.find_first_of(m_terminator, start);
        if (terminator_pos != string::npos)
        {
            m_ready_commands.push(CommandParser::ParseCommand(parse_str, start, terminator_pos));
            start = terminator_pos + 1;
        }
        else
        {
            break;
        }
    }
    
    if(start < parse_str.size())
    {
        m_unfinished_command = parse_str.substr(start);
    }
}

Command CommandParser::ParseCommand(
    const std::string& full_text,
    std::string::size_type start,
    std::string::size_type text_end)
{
    using namespace std;

    auto command_end = full_text.find_first_of(m_separator, start);
    command_end = command_end != string::npos ? command_end : text_end;
    command_end = command_end < text_end ? command_end : text_end;

    std::vector<std::string> args;

    string::size_type arg_start;
    string::size_type arg_end;
    for(arg_start = command_end + 1;
        arg_start < text_end;
        arg_start = arg_end + 1)
    {   
        arg_end = full_text.find_first_of(m_separator, arg_start);    
        if ((arg_end != string::npos) && (arg_end < text_end))    
        {
            args.push_back(full_text.substr(arg_start, arg_end - arg_start));
        }
        else
        {
            args.push_back(full_text.substr(arg_start, text_end - arg_start));
            break;
        }
    }

    return Command(full_text.substr(start, command_end - start), std::move(args));
}

bool CommandParser::HasReadyCommands() const
{
    return !m_ready_commands.empty();
}

Command CommandParser::GetReadyCommand()
{
    auto command = m_ready_commands.front();
    m_ready_commands.pop();
    return command;
}