#include "async.h"
#include "context.h"

#include "handlers/accumulate_handler.h"
#include "handlers/control_unit_handler.h"
#include "handlers/handlers_chain.h"
#include "handlers/processing_handler.h"

#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>

namespace async{
// static is used to hide the variable inside the file
static blocking_queue<std::pair<Context*, std::string>> receive_queue;

Context * connect(size_t bulk_size)
{
    return new Context(bulk_size);
}

void receive(Context * context, const char * buffer, size_t chars_count)
{
    auto commands_count = context->read_buffer_blocking(buffer, chars_count);
    for(size_t c = 0; c < commands_count; c++)
    {
        context->process_next_command_blocking();
    }
}

}