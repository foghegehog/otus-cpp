#pragma once

#include <string>

using namespace std;

/*!
 * Base command class. 
 */
class Command
{
    public:
        string Text; 
        Command(string text)
            :Text(text)
        {}
};

/*!
 * Command to be processed. 
 */
class ExecutableCommand: public Command
{
    public:
        ExecutableCommand(string text)
            :Command(text)
        {}
};

/*!
 * Command that manages bulk handling ("{", "}" or "EOF"). 
 */
class ControlCommand: public Command
{
    public:
        ControlCommand(string text)
            :Command(text)
        {}
};