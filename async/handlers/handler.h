#pragma once

#include "command.h"

#include <functional>
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
        Handler(std::function<std::unique_ptr<Handler>()> next_handler_factory)
            : m_next(std::move(next_handler_factory()))
        {}
        void Handle(const ExecutableCommand& command);
        void Handle(const ControlCommand& command);

    protected:
        virtual void ExecuteCommand(const ExecutableCommand& command) = 0;
        virtual void HandleControlFlow(const ControlCommand& command) = 0;

    private:
        std::shared_ptr<Handler> m_next;
};
}