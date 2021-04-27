#include "bulk_writer.h"
#include "logging_handler.h"

#include <chrono>
#include <climits> 
#include <iostream>

namespace postprocessing{

void LoggingHandler::Handle(std::shared_ptr<ProcessedBulk> bulk)
{
    using namespace std;

    auto id = ++m_id_generator;
    if (id == LONG_MAX - 1)
    {
        // reset counter before overflow
        m_id_generator = 1;
    }

    auto filename = m_file_prefix + to_string(bulk->m_bulk_start_time) + "_" + to_string(id) + m_file_extention;
    ofstream logfile;
    logfile.open(filename);
    BulkWriter bulk_writer(logfile);
    bulk_writer.WriteBulk(bulk->m_commands);
    logfile.close();
}

}