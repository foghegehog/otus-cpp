#ifndef ACCUMULATE_HANDLER_H
#define ACCUMULATE_HANDLER_H

#include "accumulator.h"
#include "control_unit.h"
#include "processor.h"
#include "shared_accumulator.h"

#include <memory>

namespace processing{

class AccumulateHandler
{
public:
    AccumulateHandler(
        std::shared_ptr<SharedAccumulator> static_accumulator,
        std::shared_ptr<Processor> processor)
    :m_static_accumulator(static_accumulator), m_processor(processor)
    {}

    void HandleCommand(const std::string& command);

private:
    ControlUnit m_control_unit;
    Accumulator m_dynamic_accumulator;
    std::shared_ptr<SharedAccumulator> m_static_accumulator;
    std::shared_ptr<Processor> m_processor;
    
};


}
#endif