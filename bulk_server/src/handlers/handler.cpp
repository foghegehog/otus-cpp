#include "handler.h"

namespace handlers{


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

}