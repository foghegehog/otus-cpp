#include "../../include/processing/processor.h"

#define process(bulk) (void)(bulk)

namespace processing{

void Processor::ProcessBulk(std::vector<std::string>&& bulk, time_t bulk_start)
{
    using namespace postprocessing;

    auto bulk_ptr = std::make_shared<ProcessedBulk>(move(bulk), bulk_start);
    process(bulk_ptr->m_commands);

    m_logging_queue->put(bulk_ptr);
    m_output_queue->put(bulk_ptr);

}
}