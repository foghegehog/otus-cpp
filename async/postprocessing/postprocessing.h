#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

#include "notifying_queue.h"
#include "postprocessing_handler.h"
#include "processed_bulk.h"

#include <memory>

namespace postprocessing{

class Postprocessing
{
public:
    Postprocessing(
        std::unique_ptr<PostprocessingHandler>&& handler, std::shared_ptr<notifying_queue<ProcessedBulk>> queue)
        :m_queue(queue), m_handler(std::move(handler))
    {}

    std::shared_ptr<notifying_queue<ProcessedBulk>> m_queue;

private:
    std::unique_ptr<PostprocessingHandler> m_handler;
};

}

#endif