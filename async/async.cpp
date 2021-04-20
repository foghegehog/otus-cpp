#include "async.h"
#include "context.h"

#include <cstring>
#include <functional>
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
    (void)commands_count;
}

}