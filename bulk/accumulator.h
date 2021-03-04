#pragma once

#include "command.h"

#include <vector>

using namespace std;

class Accumulator
{
    public:
        void StoreCommand(const ExecutableCommand& command);
        size_t GetCommandsStoredCount() const;
        const vector<ExecutableCommand>& GetBulk();
        void ClearBulk();
    private:
        vector<ExecutableCommand> m_bulk; 
};