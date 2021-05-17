#ifndef PROCESSING_HANDLER_H
#define PROCESSING_HANDLER_H

#include "accumulator.h"
#include "control_unit.h"
#include "handler.h"

#include <mutex>

#define process(bulk) (void)(bulk)

namespace handlers{

/*!
 * Processes ready bulk. 
 */
class ProcessingHandler: public Handler
{
    public:
        ProcessingHandler(
            std::shared_ptr<ControlUnit> controlUnit,
            std::shared_ptr<Accumulator> shared_accumulator,
            std::mutex& shared_mutex,
            std::shared_ptr<Accumulator> dynamic_accumulator,
            std::function<std::unique_ptr<Handler>()> next_handler_factory)
        : Handler(next_handler_factory),
          m_control_unit(controlUnit),
          m_shared_accumulator(shared_accumulator),
          m_shared_mutex(shared_mutex),
          m_dynamic_accumulator(dynamic_accumulator)
        {}

        void ExecuteCommand(const ExecutableCommand& command) override;
        void HandleControlFlow(const ControlCommand& command) override;
    private:
        std::shared_ptr<ControlUnit> m_control_unit;
        std::shared_ptr<Accumulator> m_shared_accumulator;
        std::mutex& m_shared_mutex;
        std::shared_ptr<Accumulator> m_dynamic_accumulator;
        void ProcessBulkIfReady();
};

}

#endif