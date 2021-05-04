#include "handlers_factory.h"

#include "handlers/accumulate_handler.h"
#include "handlers/control_unit_handler.h"
#include "handlers/processing_handler.h"

std::unique_ptr<handlers::HandlersChain> create_handlers_chain(
    const std::shared_ptr<handlers::Accumulator>& accumulator,
    const std::shared_ptr<handlers::ControlUnit>& control_unit)
{
    using namespace handlers;
    using namespace std;

    auto processing_handler_factory = [&accumulator, &control_unit]()
    {
        return make_unique<ProcessingHandler>(control_unit, accumulator, [](){ return nullptr; }); 
    };

    auto control_handler_factory = [&control_unit, processing_handler_factory]()
    {
        return make_unique<ControlUnitHandler>(control_unit, processing_handler_factory); 
    };

    auto accumulator_handler_factory = [&accumulator, control_handler_factory]()
    {
        return make_unique<AccumulateHandler>(accumulator, control_handler_factory); 
    };

    return make_unique<HandlersChain>(accumulator_handler_factory);

}