#ifndef BULK_WRITER_H
#define BULK_WRITER_H

#include <ostream>
#include <string>
#include <vector>

namespace postprocessing{

/*!
 * Outputs bulk to specified out stream.
 */
class BulkWriter
{
public:
   BulkWriter(std::ostream& stream)
   :m_stream(stream) 
   {}

   void WriteBulk(const std::vector<std::string>& bulk);
private:
   std::ostream& m_stream;
};

}
#endif

