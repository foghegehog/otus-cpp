#pragma once

#include "accumulator.h"
#include "handler.h"

/*!
 * Saves executable commands in Accumulator object. 
 */
class AccumulateHandler: public Handler
{
    public:
        AccumulateHandler(shared_ptr<Accumulator> accumulator)
        : m_accumulator(accumulator)
        {}

        void ExecuteCommand(const ExecutableCommand& command) override;
        void HandleControlFlow(const ControlCommand& command) override;
    private:
        shared_ptr<Accumulator> m_accumulator;
};