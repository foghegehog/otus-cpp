#ifndef ACCUMULATE_HANDLER_H
#define ACCUMULATE_HANDLER_H

#include <mutex>

#include "accumulator.h"
#include "control_unit.h"
#include "handler.h"

namespace handlers{

/*!
 * Saves executable commands in Accumulator object. 
 */
class AccumulateHandler: public Handler
{
public:
    AccumulateHandler(
        std::shared_ptr<ControlUnit> control_unit,
        std::shared_ptr<Accumulator> shared_accumulator,
        std::mutex& shared_mutex,
        std::shared_ptr<Accumulator> dynamic_accumulator,
        std::function<std::unique_ptr<Handler>()> next_handler_factory)
        : Handler(next_handler_factory),
        m_control_unit(control_unit),
        m_shared_accumulator(shared_accumulator),
        m_shared_mutex(shared_mutex),
        m_dynamic_accumulator(dynamic_accumulator)
    {}
    void ExecuteCommand(const ExecutableCommand& command) override;
    void HandleControlFlow(const ControlCommand& command) override;

private:
    std::shared_ptr<ControlUnit> m_control_unit;
    std::shared_ptr<Accumulator> m_shared_accumulator;
    std::mutex& m_shared_mutex;
    std::shared_ptr<Accumulator> m_dynamic_accumulator;
};

}

#endif