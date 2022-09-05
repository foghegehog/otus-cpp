#ifndef POSTPROCESSING_HANDLER_H
#define POSTPROCESSING_HANDLER_H

#include "processed_bulk.h"

#include <functional>
#include <memory>

namespace postprocessing{

class PostprocessingHandler
{
public:
    virtual void Handle(std::shared_ptr<ProcessedBulk> bulk) = 0;
};

}
#endif