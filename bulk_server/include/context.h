#ifndef CONTEXT_H
#define CONTEXT_H

#include "postprocessing/logging_handler.h"
#include "postprocessing/output_handler.h"
#include "postprocessing/notifying_queue.h"
#include "postprocessing/postprocessing.h"
#include "postprocessing/processed_bulk.h"
#include "../include/processing/accumulate_handler.h"
#include "../include/processing/processor.h"
#include "../include/processing/shared_accumulator.h"

#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace async{

void ensure_workers_started();

class Context{
public:
    Context(){};
    void process_command(const std::string& command);
    int get_id() const;
    static void set_bulk_size(size_t bulk_size);
    ~Context();

private:
    std::mutex m_processing_mutex;
    
    inline static std::shared_ptr<processing::SharedAccumulator> s_static_accumulator
        = std::make_shared<processing::SharedAccumulator>();

    inline static const postprocessing::postprocessing_queue s_logging_queue 
        = std::make_shared<postprocessing::notifying_queue<postprocessing::shared_bulk>>();
    inline static const postprocessing::postprocessing_queue s_output_queue 
        = std::make_shared<postprocessing::notifying_queue<postprocessing::shared_bulk>>();

    inline static std::shared_ptr<processing::Processor> s_static_processor 
        = std::make_shared<processing::Processor>(s_logging_queue, s_output_queue);

    static std::thread s_static_processor_thread;
    static std::thread s_logger_thread1;
    static std::thread s_logger_thread2;
    static std::thread s_output_thread;

    std::shared_ptr<processing::AccumulateHandler> m_accumulate_handler 
        = std::make_shared<processing::AccumulateHandler>(s_static_accumulator, s_static_processor);
};

}

#endif