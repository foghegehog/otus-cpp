#ifndef CONTEXT_H
#define CONTEXT_H

#include "blocking_queue.h"
#include "handlers/handlers_chain.h"
#include "handlers/accumulator.h"
#include "handlers/control_unit.h"

#include <string>
#include <vector>

namespace async{

void ensure_workers_started();

class Context{
public:
    Context(size_t bulk_size);

    size_t read_buffer_blocking(const char * buffer, size_t chars_count);
    bool process_next_command_blocking();
    void set_stopping_state();

private:
    blocking_queue<std::string> m_receive_queue;

    std::mutex m_processing_mutex;
    std::shared_ptr<handlers::Accumulator> m_accumulator;
    std::shared_ptr<handlers::ControlUnit> m_control_unit;
    std::unique_ptr<handlers::HandlersChain> m_handlers;
};

}

#endif