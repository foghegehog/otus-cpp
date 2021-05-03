#include "bulk_writer.h"

void BulkWriter::WriteBulk(const std::vector<ExecutableCommand>& bulk)
{
    m_stream << "bulk: ";
    std::string separator = "";
    for(const auto& command: bulk)
    {
        m_stream << separator << command.Text;
        separator = ", ";
    }
}