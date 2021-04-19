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

void receive(Context * context, const char * buffer, size_t chars_count)
{
    const char * buffer_end = buffer + chars_count - 1;
    const char separator = '\n';
    const char * start = skip_separator(buffer, buffer_end, separator);

    if (!start)
    {
        return;
    }

    auto is_finish = [buffer_end](const char * pos)
    { 
        return !pos || (pos > buffer_end); 
    };

    auto generator = [separator, buffer_end, context](const char *& pos){
        const char * start = pos;
        const char * cmd_end = find_separator(start, buffer_end, separator);
        auto count = cmd_end ? cmd_end - start : buffer_end - pos + 1; 
        pos = cmd_end ? skip_separator(cmd_end, buffer_end, separator) : nullptr;
        return std::make_pair(context, std::string(start, count));
    };

    receive_queue.fill<const char *>(start, generator, is_finish);
}

}