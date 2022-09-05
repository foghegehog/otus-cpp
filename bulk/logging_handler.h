#pragma once

#include "accumulator.h"
#include "control_unit.h"
#include "handler.h"

#include <ctime>
#include <fstream>
#include <string>

/*!
 * Logs ready bulk to a file. 
 */
class LoggingHandler: public Handler
{
    public:
        LoggingHandler(std::shared_ptr<ControlUnit> controlUnit, std::shared_ptr<Accumulator> accumulator)
        :m_control_unit(controlUnit), m_accumulator(accumulator)
        {}
        void ExecuteCommand(const ExecutableCommand& command) override;
        void HandleControlFlow(const ControlCommand& command) override;
    private:
        std::shared_ptr<ControlUnit> m_control_unit;
        std::shared_ptr<Accumulator> m_accumulator;
        const time_t m_unitialized_time = (time_t)(-1); 
        time_t m_bulk_time = m_unitialized_time;
        const std::string m_file_prefix = std::string("bulk"); 
        const std::string m_file_extention = std::string(".log"); 
        void LogBulkWhenReady();

};