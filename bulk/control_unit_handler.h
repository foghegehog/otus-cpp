#pragma once

#include "accumulator.h"
#include "control_unit.h"
#include "handler.h"

class ControlUnitHandler: public Handler
{
    public:
        ControlUnitHandler(shared_ptr<ControlUnit> controlUnit)
        : m_contro_unit(controlUnit)
        {}

        void ExecuteCommand(const ExecutableCommand& command) override;
        void HandleControlFlow(const ControlCommand& command) override;
    private:
        shared_ptr<ControlUnit> m_contro_unit;
};