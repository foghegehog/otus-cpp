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
    void ExecuteCommand(const ExecutableCommand& command, async::Context * context) override;
    void HandleControlFlow(const ControlCommand& command, async::Context * context) override;
};

}