#include "../include/async.h"
#include "../include/context.h"

#include "../include/handlers/accumulate_handler.h"
#include "../include/handlers/control_unit_handler.h"
#include "../include/handlers/handlers_chain.h"
#include "../include/handlers/processing_handler.h"

#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>

namespace async{

Context * connect()
{
    return new Context();
}

void receive(Context * context, const char * buffer, size_t chars_count)
{
    auto commands_count = context->read_buffer_blocking(buffer, chars_count);
    auto all_processed = false;
    for(size_t c = 0; !all_processed && (c < commands_count); c++)
    {
        all_processed = !context->process_next_command_blocking();
    }
}


void disconnect(Context * context)
{
    context->set_stopping_state();
    while(context->process_next_command_blocking())
    ;
    delete context;
}

}