#pragma once

#include <string>

namespace handlers{

/*!
 * Base command class. 
 */
class Command
{
    public:
        std::string Text; 
        Command(std::string text)
            :Text(text)
        {}
};

/*!
 * Command to be processed. 
 */
class ExecutableCommand: public Command
{
    public:
        ExecutableCommand(std::string text)
            :Command(text)
        {}
};

/*!
 * Command that manages bulk handling ("{", "}" or "EOF"). 
 */
class ControlCommand: public Command
{
    public:
        ControlCommand(std::string text)
            :Command(text)
        {}
};

}