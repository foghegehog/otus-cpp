#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

#include "notifying_queue.h"
#include "postprocessing_handler.h"
#include "processed_bulk.h"

#include <atomic>
#include <memory>

namespace postprocessing{

using shared_bulk = std::shared_ptr<ProcessedBulk>;

template<class T>
using shared_queue = std::shared_ptr<notifying_queue<T>>; 

using postprocessing_queue = shared_queue<shared_bulk>;


class Postprocessing
{
public:
    Postprocessing(
        std::unique_ptr<PostprocessingHandler>&& handler,
        postprocessing_queue queue)
        :m_queue(queue), m_handler(std::move(handler))
    {}

    void Run();

private:
    postprocessing_queue m_queue;
    std::unique_ptr<PostprocessingHandler> m_handler;
};

}

#endif