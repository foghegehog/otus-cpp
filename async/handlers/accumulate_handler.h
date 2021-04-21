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
    void ExecuteCommand(const ExecutableCommand& command, async::Context * context) override;
    void HandleControlFlow(const ControlCommand& command, async::Context * context) override;
};

}