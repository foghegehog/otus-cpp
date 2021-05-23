#include "../include/command.h"
#include "../include/command_parser.h"

#include <string>

Command CommandParser::ParseCommand(const std::string& command_text)
{
    using namespace std;
    string::size_type start = 0;
    string::size_type end = command_text.find_first_of(m_terminator);
    end = end != string::npos ? end : command_text.size(); 

    if (!end)
    {
        throw std::invalid_argument("empty command");
    }

    auto stop = command_text.find_first_of(m_separator);
    stop = stop != string::npos ? stop : end;
    Command command(command_text.substr(start, stop - start));

    while(stop != end)
    {
        start = stop + 1;
        stop = command_text.find_first_of(m_separator, start);
        stop = stop != string::npos ? stop : end;
        command.AddArgument(command_text.substr(start, stop - start));
    }
    
    return command;
}