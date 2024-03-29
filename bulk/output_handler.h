#pragma once

#include "handler.h"
#include "control_unit.h"
#include "accumulator.h"

// Using green color to differentiate console output from input
#define GREEN   "\033[32m"
#define RESET   "\033[0m"

/*!
 * Outputs ready bulk to console. 
 */
class OutputHandler: public Handler
{
    public:
        OutputHandler(std::shared_ptr<ControlUnit> controlUnit, std::shared_ptr<Accumulator> accumulator);
        void ExecuteCommand(const ExecutableCommand& command) override;
        void HandleControlFlow(const ControlCommand& command) override;
    private:
        std::shared_ptr<ControlUnit> m_control_unit;
        std::shared_ptr<Accumulator> m_accumulator; 
        void OutputBulkIfReady() const;

};