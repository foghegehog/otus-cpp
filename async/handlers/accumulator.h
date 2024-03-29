#pragma once

#include "command.h"

#include <vector>

namespace handlers{

/*!
 * Accumulutes executable commands for current bulk. 
 */
class Accumulator
{
public:
    void StoreCommand(const ExecutableCommand& command);
    size_t GetCommandsStoredCount() const;
    const std::vector<ExecutableCommand>& GetBulk();
    std::vector<ExecutableCommand>&& MoveBulk();
    void ClearBulk();
private:
    std::vector<ExecutableCommand> m_bulk; 
};

}