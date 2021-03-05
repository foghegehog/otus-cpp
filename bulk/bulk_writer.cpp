#include "bulk_writer.h"

void BulkWriter::WriteBulk(vector<ExecutableCommand> bulk)
{
    m_stream << "bulk: ";
    string separator = "";
    for(const auto& command: bulk)
    {
        m_stream << separator << command.Text;
        separator = ", ";
    }
}