#pragma once

#include "command.h"

#include <memory> 
#include <string>

#define UNUSED(expr) (void)(expr)

using namespace std;

/*!
 * Base class for Handlers implementing GoF pattern Chain of Responsibility.
 */
class Handler
{
    public:
        void Handle(const ExecutableCommand& command);
        void Handle(const ControlCommand& command);
        void SetNext(shared_ptr<Handler> next);

    protected:
        virtual void ExecuteCommand(const ExecutableCommand& command) = 0;
        virtual void HandleControlFlow(const ControlCommand& command) = 0;

    private:
        shared_ptr<Handler> m_next;
};