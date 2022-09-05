#include "../handlers/accumulator.h"

#include <gtest/gtest.h>
#include <vector>

using namespace handlers;

TEST(Accumulator, StoreCommands)
{
    Accumulator accumulator;
    size_t commands_count = 10;
    for (size_t c = 0; c < commands_count; c++)
    {
        accumulator.StoreCommand(ExecutableCommand("command"));
    }

    ASSERT_EQ(accumulator.GetCommandsStoredCount(), commands_count);
}

TEST(Accumulator, MoveCommands)
{
    Accumulator accumulator;
    size_t commands_count = 10;
    for (size_t c = 0; c < commands_count; c++)
    {
        accumulator.StoreCommand(ExecutableCommand("command"));
    }

    std::vector<ExecutableCommand> bulk = accumulator.MoveBulk();
    ASSERT_EQ(accumulator.GetCommandsStoredCount(), 0UL);
    ASSERT_EQ(bulk.size(), commands_count);
}

TEST(Accumulator, ClearCommands)
{
    Accumulator accumulator;
    size_t commands_count = 10;
    for (size_t c = 0; c < commands_count; c++)
    {
        accumulator.StoreCommand(ExecutableCommand("command"));
    }

    accumulator.ClearBulk();
    ASSERT_EQ(accumulator.GetCommandsStoredCount(), 0UL);
}