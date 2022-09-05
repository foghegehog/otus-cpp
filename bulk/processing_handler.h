#pragma once

#include "accumulator.h"
#include "control_unit.h"
#include "handler.h"

#define process(bulk) (void)(bulk)

/*!
 * Processes ready bulk. 
 */
class ProcessingHandler: public Handler
{
    public:
        ProcessingHandler(std::shared_ptr<ControlUnit> controlUnit, std::shared_ptr<Accumulator> accumulator)
        : m_contro_unit(controlUnit), m_accumulator(accumulator)
        {}

        void ExecuteCommand(const ExecutableCommand& command) override;
        void HandleControlFlow(const ControlCommand& command) override;
    private:
        std::shared_ptr<ControlUnit> m_contro_unit;
        std::shared_ptr<Accumulator> m_accumulator;
        void ProcessBulkIfReady();
};