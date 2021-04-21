#include "handler.h"

namespace handlers{

void Handler::Handle(const ExecutableCommand& command, async::Context * context)
{
    ExecuteCommand(command, context);
    if (m_next)
    {
        m_next->Handle(command, context);
    }
}

void Handler::Handle(const ControlCommand& command, async::Context * context)
{
    HandleControlFlow(command, context);
    if (m_next)
    {
        m_next->Handle(command, context);
    }
}

void Handler::SetNext(std::shared_ptr<Handler> next)
{
    m_next = std::shared_ptr<Handler>(next);
} 
}