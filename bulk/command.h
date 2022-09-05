#pragma once

#include <string>

/*!
 * Base command class. 
 */
class Command
{
    public:
        std::string Text; 
        Command(const std::string& text)
            :Text(text)
        {}
};

/*!
 * Command to be processed. 
 */
class ExecutableCommand: public Command
{
    public:
        ExecutableCommand(const std::string& text)
            :Command(text)
        {}
};

/*!
 * Command that manages bulk handling ("{", "}" or "EOF"). 
 */
class ControlCommand: public Command
{
    public:
        ControlCommand(const std::string& text)
            :Command(text)
        {}
};