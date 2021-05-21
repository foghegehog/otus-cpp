#ifndef DYNAMIC_BULK_PROCESSOR_H
#define DYNAMIC_BULK_PROCESSOR_H

#include "processor.h"

#include <string>
#include <vector>

namespace async{

class DynamicBulkProcessor : public BulkProcessor
{
public:
    void ProcessBulk();
private:;
    std::vector<std::string> m_commands;
};

}

#endif