#pragma once

#include "handler.h"

#include <functional>
#include <memory>
#include <string>

namespace handlers{

/*!
 * Containes chain of command handlers.
 */
class HandlersChain
{
    public:
        HandlersChain(std::function<std::unique_ptr<Handler>()> front_handler_factory)
            : m_front_handler(front_handler_factory())
        {}
        void PassThrough(std::string command_text); 
    private:
        std::unique_ptr<Handler> m_front_handler;
};

}