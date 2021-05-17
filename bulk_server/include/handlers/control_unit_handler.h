#pragma once

#include "accumulator.h"
#include "threadsafe_accumulator.h"
#include "control_unit.h"
#include "handler.h"

namespace handlers{

/*!
 * Passes command-handling events to ControlUnit to chane it's state. 
 */
class ControlUnitHandler: public Handler
{
public:
    ControlUnitHandler(
        std::shared_ptr<ControlUnit> control_unit,
        std::shared_ptr<ThreadSafeAccumulator> static_accumulator,
        std::function<std::unique_ptr<Handler>()> next_handler_factory)
        :Handler(next_handler_factory), m_control_unit(control_unit), m_static_accumulator(static_accumulator)
    {}
    void ExecuteCommand(const ExecutableCommand& command) override;
    void HandleControlFlow(const ControlCommand& command) override;

private:
    std::shared_ptr<ControlUnit> m_control_unit;
    std::shared_ptr<ThreadSafeAccumulator> m_static_accumulator;
};

}