#include "handler.h"

void Handler::Handle(const ExecutableCommand& command)
{
    ExecuteCommand(command);
    if (m_next)
    {
        m_next->Handle(command);
    }
}

void Handler::Handle(const ControlCommand& command)
{
    HandleControlFlow(command);
    if (m_next)
    {
        m_next->Handle(command);
    }
}

void Handler::SetNext(std::shared_ptr<Handler> next)
{
    m_next = next;
} 