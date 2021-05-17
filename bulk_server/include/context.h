#ifndef CONTEXT_H
#define CONTEXT_H

#include "blocking_queue.h"
#include "handlers/handlers_chain.h"
#include "handlers/accumulator.h"
#include "handlers/control_unit.h"
#include "postprocessing/logging_handler.h"
#include "postprocessing/output_handler.h"
#include "postprocessing/notifying_queue.h"
#include "postprocessing/postprocessing.h"
#include "postprocessing/processed_bulk.h"

#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace async{

void ensure_workers_started();

class Context{
public:
    Context(size_t bulk_size);

    size_t read_buffer_blocking(const char * buffer, size_t chars_count);
    bool process_next_command_blocking();
    void set_stopping_state();
    
    static void stop_background_workers(); 

private:
    blocking_queue<std::string> m_receive_queue;

    std::mutex m_processing_mutex;
    std::shared_ptr<handlers::Accumulator> m_shared_accumulator;
    std::mutex& m_shared_accumulator_mutex;
    std::shared_ptr<handlers::Accumulator> m_dynamic_accumulator;
    std::shared_ptr<handlers::ControlUnit> m_control_unit;
    std::unique_ptr<handlers::HandlersChain> m_handlers;

    inline static const std::shared_ptr<postprocessing::notifying_queue<std::shared_ptr<postprocessing::ProcessedBulk>>> s_logging_queue 
        = std::make_shared<postprocessing::notifying_queue<std::shared_ptr<postprocessing::ProcessedBulk>>>();
    inline static const std::shared_ptr<postprocessing::notifying_queue<std::shared_ptr<postprocessing::ProcessedBulk>>> s_output_queue 
        = std::make_shared<postprocessing::notifying_queue<std::shared_ptr<postprocessing::ProcessedBulk>>>();

    static std::thread s_logger_thread1;
    static std::thread s_logger_thread2;
    static std::thread s_output_thread;
};

}

#endif