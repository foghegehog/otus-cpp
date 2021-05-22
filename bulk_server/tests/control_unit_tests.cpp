
#include <gtest/gtest.h>

/*
TEST(ControlUnit, InitialState)
{
    const size_t bulk_size = 5;
    ControlUnit control_unit(bulk_size);
    ASSERT_EQ(control_unit.GetStaticBulkSize(), bulk_size);
    ASSERT_EQ(control_unit.GetState(), ControlUnit::State::Empty);
}

TEST(ControlUnit, UnfinishedBulk)
{
    const size_t bulk_size = 5;
    ControlUnit control_unit(bulk_size);
    for(size_t c = 0; c < bulk_size; c++)
    {
        control_unit.HandleEvent(ControlUnit::CommandAdded);
    }

    ASSERT_EQ(control_unit.GetState(), ControlUnit::State::GatheringStatic);
    ASSERT_FALSE(control_unit.ShouldProcessStaticBulk());
    ASSERT_FALSE(control_unit.ShouldClearStaticBulk());
}

TEST(ControlUnit, StaticBulkReady)
{
    const size_t bulk_size = 5;
    ControlUnit control_unit(bulk_size);
    for(size_t c = 0; c < bulk_size; c++)
    {
        control_unit.HandleEvent(ControlUnit::CommandAdded);
    }

    control_unit.HandleEvent(ControlUnit::StaticBulkSizeReached);

    ASSERT_EQ(control_unit.GetState(), ControlUnit::State::StaticBulkReady);
    ASSERT_TRUE(control_unit.ShouldProcessStaticBulk());
    ASSERT_FALSE(control_unit.ShouldClearStaticBulk());
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
    ASSERT_TRUE(control_unit.ShouldProcessStaticBulk());
    ASSERT_FALSE(control_unit.ShouldClearStaticBulk());
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
    ASSERT_FALSE(control_unit.ShouldProcessStaticBulk());
    ASSERT_TRUE(control_unit.ShouldClearStaticBulk());
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
    ASSERT_FALSE(control_unit.ShouldProcessStaticBulk());
    ASSERT_FALSE(control_unit.ShouldClearStaticBulk());
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

    ASSERT_EQ(control_unit.GetState(), ControlUnit::State::DynamicBulkReady);
    ASSERT_FALSE(control_unit.ShouldProcessStaticBulk());
    ASSERT_FALSE(control_unit.ShouldClearStaticBulk());
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
    ASSERT_FALSE(control_unit.ShouldProcessStaticBulk());
    ASSERT_FALSE(control_unit.ShouldClearStaticBulk());
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

    ASSERT_EQ(control_unit.GetState(), ControlUnit::State::StaticBulkReady);
    ASSERT_TRUE(control_unit.ShouldProcessStaticBulk());
    ASSERT_FALSE(control_unit.ShouldClearStaticBulk());
}
*/
int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}