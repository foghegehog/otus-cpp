#include "../include/commands_parsing.h"

namespace commands_parsing{

const char * skip_separator(const char * buffer, const char * buffer_end, char separator)
{
    const char * skip = buffer;
    while(skip != buffer_end && (*skip == separator))
    {
        skip++;
    }

    if (*skip != separator)
    {
        return skip;
    }
    else
    {
        return nullptr;
    }
}

const char * find_separator(const char * buffer, const char * buffer_end, char separator)
{
    const char * pos = buffer;
    while(pos != buffer_end && (*pos != separator))
    {
        pos++;
    }

    if (*pos == separator)
    {
        return pos;
    }
    else
    {
        return nullptr;
    }
}

bool is_buffer_end(const char * pos, const char * buffer_end)
{
    return !pos || (pos > buffer_end);
}

std::string extract_command(const char *& pos, const char * buffer_end, char separator)
{
    const char * start = pos;
    const char * cmd_end = find_separator(start, buffer_end, separator);
    auto count = cmd_end ? cmd_end - start : buffer_end - pos + 1; 
    pos = cmd_end ? skip_separator(cmd_end, buffer_end, separator) : nullptr;
    return std::string(start, count);
}

}