#pragma once

#include "command.h"

#include <vector>

/*!
 * Accumulutes executable commands for current bulk. 
 */
class Accumulator
{
    public:
        void StoreCommand(const ExecutableCommand& command);
        size_t GetCommandsStoredCount() const;
        const std::vector<ExecutableCommand>& GetBulk();
        void ClearBulk();
    private:
        std::vector<ExecutableCommand> m_bulk; 
};