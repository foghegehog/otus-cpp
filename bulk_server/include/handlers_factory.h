#ifndef HANDLERS_FACTORY_H
#define HANDLERS_FACTORY_H

#include <memory>
#include <mutex>

#include "handlers/accumulator.h"
#include "handlers/control_unit.h"
#include "handlers/handlers_chain.h"

std::unique_ptr<handlers::HandlersChain> create_handlers_chain(
    const std::shared_ptr<handlers::Accumulator>& static_accumulator,
    std::mutex& shared_mutex,
    const std::shared_ptr<handlers::Accumulator>& dynamic_accumulator,
    const std::shared_ptr<handlers::ControlUnit>& control_unit); 

#endif