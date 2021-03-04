#include "handlers_chain.h"

void HandlersChain::PassThrough(string command_text)
{
    if ((command_text == "{") || (command_text == "}"))
    {
        ControlCommand command(command_text);
        m_front_handler->Handle(command);
    }
    else if (command_text == "EOF")
    {
        ControlCommand command("EOF");
        m_front_handler->Handle(command);
    }
    else
    {
        ExecutableCommand command(command_text);
        m_front_handler->Handle(command);
    }
}

void HandlersChain::AddFront(shared_ptr<Handler> handler)
{
    handler->SetNext(m_front_handler);
    m_front_handler = handler;
}