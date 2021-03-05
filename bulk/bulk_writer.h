#pragma once

#include "command.h"

#include <ostream>
#include <vector>

using namespace std;

class BulkWriter
{
    public:
       BulkWriter(ostream& stream)
       :m_stream(stream) 
       {}

       void WriteBulk(vector<ExecutableCommand> bulk);
    private:
       ostream& m_stream;
};