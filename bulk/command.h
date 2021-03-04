#pragma once

#include <string>

using namespace std;

class Command
{
    public:
        string Text; 
        Command(string text)
            :Text(text)
        {}
};

class ExecutableCommand: public Command
{
    public:
        ExecutableCommand(string text)
            :Command(text)
        {}
};

class ControlCommand: public Command
{
    public:
        ControlCommand(string text)
            :Command(text)
        {}
};