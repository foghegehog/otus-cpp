#pragma once

#include "accumulator.h"
#include "control_unit.h"
#include "handler.h"

#include <ctime>
#include <fstream>
#include <string>

using namespace std;

/*!
 * Logs ready bulk to a file. 
 */
class LoggingHandler: public Handler
{
    public:
        LoggingHandler(shared_ptr<ControlUnit> controlUnit, shared_ptr<Accumulator> accumulator)
        :m_control_unit(controlUnit), m_accumulator(accumulator)
        {}
        void ExecuteCommand(const ExecutableCommand& command) override;
        void HandleControlFlow(const ControlCommand& command) override;
    private:
        shared_ptr<ControlUnit> m_control_unit;
        shared_ptr<Accumulator> m_accumulator;
        const time_t m_unitialized_time = (time_t)(-1); 
        time_t m_bulk_time = m_unitialized_time;
        const string m_file_prefix = string("bulk"); 
        const string m_file_extention = string(".log"); 
        void LogBulkWhenReady();

};