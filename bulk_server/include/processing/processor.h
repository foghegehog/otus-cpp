#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "../../include/postprocessing/notifying_queue.h"
#include "../../include/postprocessing/processed_bulk.h"
#include "../../include/postprocessing/postprocessing.h"

#include <memory>
#include <string>
#include <vector>

namespace processing
{

class Processor
{
public:
    Processor(
        postprocessing::postprocessing_queue logging_queue,
        postprocessing::postprocessing_queue output_queue)
        :m_logging_queue(logging_queue), m_output_queue(output_queue)
        {};
    void ProcessBulk(std::vector<std::string>&& bulk, time_t bulk_start);
private:
    postprocessing::postprocessing_queue m_logging_queue;
    postprocessing::postprocessing_queue m_output_queue;   
     
};

} 
#endif

