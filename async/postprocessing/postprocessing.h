#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

#include "../blocking_queue.h"
#include "postprocessing_handler.h"
#include "processed_bulk.h"

#include <memory>

namespace postprocessing{

inline static const ushort POSTPROCESSING_HANDLERS = 2;

class Postprocessing
{
public:
    Postprocessing(
        std::unique_ptr<PostprocessingHandler>&& handler, std::shared_ptr<blocking_queue<ProcessedBulk>> queue)
        :m_queue(queue), m_handler(std::move(handler))
    {}

    std::shared_ptr<blocking_queue<ProcessedBulk>> m_queue;

private:
    std::unique_ptr<PostprocessingHandler> m_handler;
};

}

#endif