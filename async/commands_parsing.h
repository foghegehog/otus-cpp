#ifndef COMMAND_PARSING_H
#define COMMAND_PARSING_H

#include <string>

namespace commands_parsing{

const char * skip_separator(const char * buffer, const char * buffer_end, char separator);

const char * find_separator(const char * buffer, const char * buffer_end, char separator);

bool is_buffer_end(const char * pos, const char * buffer_end);

std::string extract_command(const char *& pos, const char * buffer_end, char separator);

}
#endif