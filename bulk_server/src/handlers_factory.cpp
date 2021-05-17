#include "../include/handlers_factory.h"

#include "../include/handlers/accumulate_handler.h"
#include "../include/handlers/control_unit_handler.h"
#include "../include/handlers/processing_handler.h"

std::unique_ptr<handlers::HandlersChain> create_handlers_chain(
    const std::shared_ptr<handlers::Accumulator>& shared_accumulator,
    std::mutex& shared_mutex,
    const std::shared_ptr<handlers::Accumulator>& dynamic_accumulator,
    const std::shared_ptr<handlers::ControlUnit>& control_unit)
{
    using namespace handlers;
    using namespace std;

    auto processing_handler_factory = [&shared_accumulator, &shared_mutex, &dynamic_accumulator, &control_unit]()
    {
        return make_unique<ProcessingHandler>(
            control_unit, shared_accumulator, shared_mutex, dynamic_accumulator, [](){ return nullptr; }); 
    };

    auto control_handler_factory = [&control_unit, &shared_accumulator, &shared_mutex, processing_handler_factory]()
    {
        return make_unique<ControlUnitHandler>(control_unit, shared_accumulator, shared_mutex, processing_handler_factory); 
    };

    auto accumulator_handler_factory = [&control_unit, &dynamic_accumulator, &shared_accumulator, &shared_mutex, control_handler_factory]()
    {
        return make_unique<AccumulateHandler>(
            control_unit, shared_accumulator, shared_mutex, dynamic_accumulator, control_handler_factory); 
    };

    return make_unique<HandlersChain>(accumulator_handler_factory);

}