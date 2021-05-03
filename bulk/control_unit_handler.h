#pragma once

#include "accumulator.h"
#include "control_unit.h"
#include "handler.h"

/*!
 * Passes command-handling events to ControlUnit to chane it's state. 
 */
class ControlUnitHandler: public Handler
{
    public:
        ControlUnitHandler(std::shared_ptr<ControlUnit> controlUnit)
        : m_contro_unit(controlUnit)
        {}

        void ExecuteCommand(const ExecutableCommand& command) override;
        void HandleControlFlow(const ControlCommand& command) override;
    private:
        std::shared_ptr<ControlUnit> m_contro_unit;
};