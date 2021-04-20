#ifndef CONTEXT_H
#define CONTEXT_H

#include "blocking_queue.h"

#include <string>
#include <vector>

namespace async{


class Context{
public:
    Context(size_t bulk_size)
        :m_commands(bulk_size)
    {
    }

    size_t read_buffer(const char * buffer, size_t chars_count);

private:
    blocking_queue<std::string> m_receive_queue;

    std::vector<std::string> m_commands;
};

}

#endif