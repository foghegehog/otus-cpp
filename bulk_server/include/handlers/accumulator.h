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
    virtual void StoreCommand(const ExecutableCommand& command);
    virtual size_t GetCommandsStoredCount();
    virtual std::vector<ExecutableCommand>&& MoveBulk();
    virtual const std::vector<ExecutableCommand>& GetBulk();
    virtual void ClearBulk();
private:
    std::vector<ExecutableCommand> m_bulk; 
};

}