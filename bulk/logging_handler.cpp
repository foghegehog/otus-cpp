#include "bulk_writer.h"
#include "logging_handler.h"

#include <chrono>
#include <iostream>

using namespace std;

void LoggingHandler::ExecuteCommand(const ExecutableCommand& command)
{
    UNUSED(command);

    if (m_bulk_time == m_unitialized_time)
    {
        auto now = chrono::system_clock::now(); 
        m_bulk_time = chrono::system_clock::to_time_t(now);
    }

    LogBulkWhenReady();
}

void LoggingHandler::HandleControlFlow(const ControlCommand& command)
{
    UNUSED(command);
    LogBulkWhenReady();
}

void LoggingHandler::LogBulkWhenReady()
{
    if (m_control_unit->ShouldProcessBulk())
    {
        auto bulk = m_accumulator->GetBulk();

        auto filename = m_file_prefix + to_string(m_bulk_time) + m_file_extention;
        //cout << filename << endl;
        ofstream logfile;
        logfile.open(filename);
        BulkWriter bulk_writer(logfile);
        bulk_writer.WriteBulk(bulk);
        logfile.close();

        m_bulk_time = m_unitialized_time;
    }
}