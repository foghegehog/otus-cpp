#pragma once

#include "handler.h"

#include <memory>
#include <string>

/*!
 * Containes chain of command handlers.
 */
class HandlersChain
{
    public:
        HandlersChain(std::shared_ptr<Handler> front_handler)
        :m_front_handler(front_handler)
        {}

        void AddFront(std::shared_ptr<Handler> handler);
        void PassThrough(const std::string& command_text); 
    private:
        std::shared_ptr<Handler> m_front_handler;
};