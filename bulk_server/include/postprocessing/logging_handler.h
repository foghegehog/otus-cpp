#ifndef LOGGING_HANDLER_H
#define LOGGING_HANDLER_H

#include "postprocessing_handler.h"

#include <atomic>
#include <ctime>
#include <fstream>
#include <memory>
#include <string>

namespace postprocessing{

/*!
 * Logs ready bulk to a file. 
 */
class LoggingHandler: public PostprocessingHandler
{
public:
    LoggingHandler(std::string name)
        :m_file_prefix(name + "_bulk")
    {
    }

    void Handle(std::shared_ptr<ProcessedBulk> bulk) override;

private:
    std::string m_file_prefix; 
    const std::string m_file_extention = std::string(".log");
    ulong m_id_generator = 0;
};
}

#endif