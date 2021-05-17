#include "../../include/handlers/accumulator.h"


namespace handlers{

void Accumulator::StoreCommand(const ExecutableCommand& command)
{
    m_bulk.emplace_back(command);
}

size_t Accumulator::GetCommandsStoredCount()
{
    return m_bulk.size();
}

const std::vector<ExecutableCommand>& Accumulator::GetBulk()
{
    return m_bulk;
}

std::vector<ExecutableCommand>&& Accumulator::MoveBulk()
{
    return std::move(m_bulk);
}

void Accumulator::ClearBulk()
{
    m_bulk.clear();
}
}