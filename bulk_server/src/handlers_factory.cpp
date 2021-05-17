#include "../include/handlers_factory.h"

#include "../include/handlers/accumulate_handler.h"
#include "../include/handlers/threadsafe_accumulator.h"
#include "../include/handlers/control_unit_handler.h"
#include "../include/handlers/processing_handler.h"
#include "../include/handlers/threadsafe_accumulator.h"

std::unique_ptr<handlers::HandlersChain> create_handlers_chain(
    const std::shared_ptr<handlers::ThreadSafeAccumulator>& static_accumulator,
    const std::shared_ptr<handlers::Accumulator>& dynamic_accumulator,
    const std::shared_ptr<handlers::ControlUnit>& control_unit)
{
    using namespace handlers;
    using namespace std;

    auto processing_handler_factory = [&static_accumulator, &dynamic_accumulator, &control_unit]()
    {
        return make_unique<ProcessingHandler>(control_unit, static_accumulator, dynamic_accumulator, [](){ return nullptr; }); 
    };

    auto control_handler_factory = [&control_unit, &static_accumulator, processing_handler_factory]()
    {
        return make_unique<ControlUnitHandler>(control_unit, static_accumulator, processing_handler_factory); 
    };

    auto accumulator_handler_factory = [&control_unit, &dynamic_accumulator, &static_accumulator, control_handler_factory]()
    {
        return make_unique<AccumulateHandler>(
            control_unit, static_accumulator, dynamic_accumulator, control_handler_factory); 
    };

    return make_unique<HandlersChain>(accumulator_handler_factory);

}