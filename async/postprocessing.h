#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

#include "blocking_queue.h"
#include "handlers/command.h"

#include <chrono>
#include <vector>

namespace async{

class ProcessedBulk
{
private:
    std::vector<handlers::ExecutableCommand> m_commands; 
    time_t start_time;
    
};

class Postprocessing
{

private:
    blocking_queue<ProcessedBulk> m_queue;
};

}

#endif