#pragma once

#include "handler.h"
#include "control_unit.h"
#include "accumulator.h"

#define GREEN   "\033[32m"
#define RESET   "\033[0m"

class OutputHandler: public Handler
{
    public:
        OutputHandler(shared_ptr<ControlUnit> controlUnit, shared_ptr<Accumulator> accumulator);
        void ExecuteCommand(const ExecutableCommand& command) override;
        void HandleControlFlow(const ControlCommand& command) override;
    private:
        shared_ptr<ControlUnit> m_control_unit;
        shared_ptr<Accumulator> m_accumulator; 
        void OutputBulkIfReady() const;

};