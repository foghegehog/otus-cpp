#pragma once

#include "handler.h"

#include <memory>
#include <string>

using namespace std;

/*!
 * Containes chain of command handlers.
 */
class HandlersChain
{
    public:
        HandlersChain(shared_ptr<Handler> front_handler)
        :m_front_handler(front_handler)
        {}

        void AddFront(shared_ptr<Handler> handler);
        void PassThrough(string command_text); 
    private:
        shared_ptr<Handler> m_front_handler;
};