#include "bulk_writer.h"

namespace postprocessing{

void BulkWriter::WriteBulk(const std::vector<handlers::ExecutableCommand>& bulk)
{
    m_stream << "bulk: ";
    std::string separator = "";
    for(const auto& command: bulk)
    {
        m_stream << separator << command.Text;
        separator = ", ";
    }
}
}