#include "../../include/postprocessing/bulk_writer.h"

namespace postprocessing{

void BulkWriter::WriteBulk(const std::vector<std::string>& bulk)
{
    m_stream << "bulk: ";
    std::string separator = "";
    for(const auto& command: bulk)
    {
        m_stream << separator << command;
        separator = ", ";
    }
}
}