#include "../include/async.h"
#include "../include/commands_parsing.h"
#include "../include/context.h"

#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <utility>

namespace async{

Context * connect()
{
    return new Context();
}

void receive(Context * context, const char * buffer, size_t chars_count)
{
    std::vector<std::string> commands;
    const char * buffer_end = buffer + chars_count - 1;
    const char separator = '\n';
    const char * pos = commands_parsing::skip_separator(buffer, buffer_end, separator);
    while(!commands_parsing::is_buffer_end(pos, buffer_end))
    {
        auto command = commands_parsing::extract_command(pos, buffer_end, separator);
        context->process_command(command);
    }
}


void disconnect(Context * context)
{
    delete context;
}

}