#pragma once

#include "accumulator.h"
#include "control_unit.h"
#include "handler.h"

#define process(bulk) (void)(bulk)

namespace handlers{

/*!
 * Processes ready bulk. 
 */
class ProcessingHandler: public Handler
{
    public:
        ProcessingHandler(std::shared_ptr<ControlUnit> controlUnit, std::shared_ptr<Accumulator> accumulator)
        : m_contro_unit(controlUnit), m_accumulator(accumulator)
        {}

        void ExecuteCommand(const ExecutableCommand& command, async::Context * context) override;
        void HandleControlFlow(const ControlCommand& command, async::Context * context) override;
    private:
        std::shared_ptr<ControlUnit> m_contro_unit;
        std::shared_ptr<Accumulator> m_accumulator;
        void ProcessBulkIfReady(async::Context * context);
};

}