#include <atomic>

#include "../../include/postprocessing/postprocessing.h"

namespace postprocessing{

void Postprocessing::Run()
{
    std::shared_ptr<ProcessedBulk> bulk;
    while(m_queue->try_pop(bulk))
    {
        m_handler->Handle(bulk);
    }
}

}