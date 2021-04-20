#include "async.h"
#include "context.h"

#include <cstring>
#include <functional>
#include <iostream>
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
    auto commands_count = context->read_buffer(buffer, chars_count);
    for(size_t c = 0; c < commands_count; c++)
    {
        {
            std::lock_guard<std::mutex> lock(context->m_command_mutex);
            auto command = context->dequeue_command();
            std::cout << "thread: " << std::this_thread::get_id() << " cmd: " << command << std::endl; 
        }
    }
}

}