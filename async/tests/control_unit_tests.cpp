#include "../handlers/control_unit.h"

#include <gtest/gtest.h>

using namespace handlers;

TEST(ControlUnit, InitialState)
{
    ControlUnit control_unit(5);
    ASSERT_EQ(control_unit.GetState(), ControlUnit::State::Empty);
}

TEST(ControlUnit, UnfinishedBulk)
{
    const size_t bulk_size = 5;
    ControlUnit control_unit(bulk_size);
    for(size_t c = 0; c < bulk_size - 1; c++)
    {
        control_unit.HandleEvent(ControlUnit::CommandAdded);
    }

    ASSERT_EQ(control_unit.GetState(), ControlUnit::State::GatheringStatic);
    ASSERT_FALSE(control_unit.ShouldProcessBulk());
    ASSERT_FALSE(control_unit.ShouldClearProcessedBulk());
}

TEST(ControlUnit, StaticBulkReady)
{
    const size_t bulk_size = 5;
    ControlUnit control_unit(bulk_size);
    for(size_t c = 0; c < bulk_size; c++)
    {
        control_unit.HandleEvent(ControlUnit::CommandAdded);
    }

    ASSERT_EQ(control_unit.GetState(), ControlUnit::State::BulkReady);
    ASSERT_TRUE(control_unit.ShouldProcessBulk());
    ASSERT_FALSE(control_unit.ShouldClearProcessedBulk());
}

TEST(ControlUnit, DynamicBulkOpened)
{
    const size_t bulk_size = 5;
    ControlUnit control_unit(bulk_size);
    for(size_t c = 0; c < bulk_size - 1; c++)
    {
        control_unit.HandleEvent(ControlUnit::CommandAdded);
    }

    control_unit.HandleEvent(ControlUnit::BlockOpened);

    ASSERT_EQ(control_unit.GetState(), ControlUnit::State::ProcessUnfinished);
    ASSERT_TRUE(control_unit.ShouldProcessBulk());
    ASSERT_FALSE(control_unit.ShouldClearProcessedBulk());
}

TEST(ControlUnit, UnfinishedStaticProcessed)
{
    const size_t bulk_size = 5;
    ControlUnit control_unit(bulk_size);
    for(size_t c = 0; c < bulk_size - 1; c++)
    {
        control_unit.HandleEvent(ControlUnit::CommandAdded);
    }

    control_unit.HandleEvent(ControlUnit::BlockOpened);
    control_unit.HandleEvent(ControlUnit::BulkProcessed);

    ASSERT_EQ(control_unit.GetState(), ControlUnit::State::ClearProcessedUnfinished);
    ASSERT_FALSE(control_unit.ShouldProcessBulk());
    ASSERT_TRUE(control_unit.ShouldClearProcessedBulk());
}

TEST(ControlUnit, GatheringDynamicBulk)
{
    const size_t bulk_size = 5;
    ControlUnit control_unit(bulk_size);
    for(size_t c = 0; c < bulk_size - 1; c++)
    {
        control_unit.HandleEvent(ControlUnit::CommandAdded);
    }

    control_unit.HandleEvent(ControlUnit::BlockOpened);
    control_unit.HandleEvent(ControlUnit::BulkProcessed);
    control_unit.HandleEvent(ControlUnit::ClearedProcessed);
    control_unit.HandleEvent(ControlUnit::CommandAdded);

    ASSERT_EQ(control_unit.GetState(), ControlUnit::State::GatheringDynamic);
    ASSERT_FALSE(control_unit.ShouldProcessBulk());
    ASSERT_FALSE(control_unit.ShouldClearProcessedBulk());
}

TEST(ControlUnit, DynamicBulkClosed)
{
    const size_t bulk_size = 5;
    ControlUnit control_unit(bulk_size);
    for(size_t c = 0; c < bulk_size - 1; c++)
    {
        control_unit.HandleEvent(ControlUnit::CommandAdded);
    }

    control_unit.HandleEvent(ControlUnit::BlockOpened);
    control_unit.HandleEvent(ControlUnit::BulkProcessed);
    control_unit.HandleEvent(ControlUnit::ClearedProcessed);
    control_unit.HandleEvent(ControlUnit::CommandAdded);
    control_unit.HandleEvent(ControlUnit::BlockClosed);

    ASSERT_EQ(control_unit.GetState(), ControlUnit::State::BulkReady);
    ASSERT_TRUE(control_unit.ShouldProcessBulk());
    ASSERT_FALSE(control_unit.ShouldClearProcessedBulk());
}

TEST(ControlUnit, DiscardUnfinishedDynamicOnEOF)
{
    const size_t bulk_size = 5;
    ControlUnit control_unit(bulk_size);
    for(size_t c = 0; c < bulk_size - 1; c++)
    {
        control_unit.HandleEvent(ControlUnit::CommandAdded);
    }

    control_unit.HandleEvent(ControlUnit::BlockOpened);
    control_unit.HandleEvent(ControlUnit::BulkProcessed);
    control_unit.HandleEvent(ControlUnit::ClearedProcessed);
    control_unit.HandleEvent(ControlUnit::CommandAdded);
    control_unit.HandleEvent(ControlUnit::EndOfFile);

    ASSERT_EQ(control_unit.GetState(), ControlUnit::State::Discard);
    ASSERT_FALSE(control_unit.ShouldProcessBulk());
    ASSERT_FALSE(control_unit.ShouldClearProcessedBulk());
}

TEST(ControlUnit, ProcessUnfinishedStaticOnEOF)
{
    const size_t bulk_size = 5;
    ControlUnit control_unit(bulk_size);
    for(size_t c = 0; c < bulk_size - 1; c++)
    {
        control_unit.HandleEvent(ControlUnit::CommandAdded);
    }

    control_unit.HandleEvent(ControlUnit::EndOfFile);

    ASSERT_EQ(control_unit.GetState(), ControlUnit::State::BulkReady);
    ASSERT_TRUE(control_unit.ShouldProcessBulk());
    ASSERT_FALSE(control_unit.ShouldClearProcessedBulk());
}