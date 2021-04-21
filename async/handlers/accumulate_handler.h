#pragma once

#include "accumulator.h"
#include "handler.h"

namespace handlers{

/*!
 * Saves executable commands in Accumulator object. 
 */
class AccumulateHandler: public Handler
{
    public:
        AccumulateHandler(std::shared_ptr<Accumulator> accumulator)
        : m_accumulator(accumulator)
        {}

        void ExecuteCommand(const ExecutableCommand& command, async::Context * context) override;
        void HandleControlFlow(const ControlCommand& command, async::Context * context) override;
    private:
        std::shared_ptr<Accumulator> m_accumulator;
};

}