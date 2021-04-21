#ifndef CONTEXT_H
#define CONTEXT_H

#include "blocking_queue.h"
#include "handlers/accumulator.h"
#include "handlers/control_unit.h"

#include <string>
#include <vector>

namespace async{


class Context{
public:
    Context(size_t bulk_size)
        :m_control_unit(bulk_size)
    {
    }

    size_t read_buffer(const char * buffer, size_t chars_count);
    std::string dequeue_command();

    std::mutex m_command_mutex;

    handlers::Accumulator m_accumulator;
    handlers::ControlUnit m_control_unit;

private:
    blocking_queue<std::string> m_receive_queue;
};

}

#endif