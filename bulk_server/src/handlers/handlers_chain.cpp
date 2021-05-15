#include "../../include/handlers/handlers_chain.h"

namespace handlers{

void HandlersChain::PassThrough(std::string command_text)
{
    if ((command_text == ControlCommand::BLOCK_START) 
     || (command_text == ControlCommand::BLOCK_END))
    {
        ControlCommand command(command_text);
        m_front_handler->Handle(command);
    }
    else if (command_text == ControlCommand::EOF_COMMAND)
    {
        ControlCommand command(ControlCommand::EOF_COMMAND);
        m_front_handler->Handle(command);
    }
    else
    {
        ExecutableCommand command(command_text);
        m_front_handler->Handle(command);
    }
}

}