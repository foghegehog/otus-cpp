#include "accumulator.h"

using namespace std;

void Accumulator::StoreCommand(const ExecutableCommand& command)
{
    m_bulk.emplace_back(command);
}

size_t Accumulator::GetCommandsStoredCount() const
{
    return m_bulk.size();
}

const vector<ExecutableCommand>& Accumulator::GetBulk()
{
    return m_bulk;
}

void Accumulator::ClearBulk()
{
    m_bulk.clear();
}