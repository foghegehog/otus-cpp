#include "handlers_chain.h"

namespace handlers{

void HandlersChain::PassThrough(std::string command_text, async::Context * context)
{
    if ((command_text == "{") || (command_text == "}"))
    {
        ControlCommand command(command_text);
        m_front_handler->Handle(command, context);
    }
    else if (command_text == "EOF")
    {
        ControlCommand command("EOF");
        m_front_handler->Handle(command, context);
    }
    else
    {
        ExecutableCommand command(command_text);
        m_front_handler->Handle(command, context);
    }
}

void HandlersChain::AddFront(std::shared_ptr<Handler> handler)
{
    handler->SetNext(m_front_handler);
    m_front_handler = handler;
}

}