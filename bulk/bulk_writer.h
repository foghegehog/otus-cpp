#pragma once

#include "command.h"

#include <ostream>
#include <vector>

/*!
 * Outputs bulk to specified out stream.
 */
class BulkWriter
{
    public:
       BulkWriter(std::ostream& stream)
       :m_stream(stream) 
       {}

       void WriteBulk(const std::vector<ExecutableCommand>& bulk);
    private:
      std::ostream& m_stream;
};