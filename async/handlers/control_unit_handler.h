#pragma once

#include "accumulator.h"
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
        std::function<std::unique_ptr<Handler>()> next_handler_factory)
        :Handler(next_handler_factory), m_control_unit(control_unit)
    {}
    void ExecuteCommand(const ExecutableCommand& command) override;
    void HandleControlFlow(const ControlCommand& command) override;

private:
    std::shared_ptr<ControlUnit> m_control_unit;
};

}