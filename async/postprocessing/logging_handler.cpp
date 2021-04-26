#include "bulk_writer.h"
#include "logging_handler.h"

#include <chrono>
#include <iostream>

namespace postprocessing{

void LoggingHandler::Handle(const ProcessedBulk& bulk)
{
    using namespace std;

    auto filename = m_file_prefix + to_string(bulk.m_bulk_start_time) + m_file_extention;
    //cout << filename << endl;
    ofstream logfile;
    logfile.open(filename);
    BulkWriter bulk_writer(logfile);
    bulk_writer.WriteBulk(bulk.m_commands);
    logfile.close();
}

}