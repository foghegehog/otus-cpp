#pragma once

#include "command.h"
#include "../context.h"

#include <memory> 
#include <string>

#define UNUSED(expr) (void)(expr)

namespace handlers{

/*!
 * Base class for Handlers implementing GoF pattern Chain of Responsibility.
 */
class Handler
{
    public:
        void Handle(const ExecutableCommand& command, async::Context * context);
        void Handle(const ControlCommand& command, async::Context * context);
        void SetNext(std::shared_ptr<Handler> next);

    protected:
        virtual void ExecuteCommand(const ExecutableCommand& command, async::Context * context) = 0;
        virtual void HandleControlFlow(const ControlCommand& command, async::Context * context) = 0;

    private:
        std::shared_ptr<Handler> m_next;
};
}