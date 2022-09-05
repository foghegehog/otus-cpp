
#include "../include/processing/control_unit.h"

#include <gtest/gtest.h>

using namespace processing;

TEST(ControlUnit, InitialState)
{
    ControlUnit control_unit;
    ASSERT_EQ(control_unit.GetState(), ControlUnit::State::Empty);
}

TEST(ControlUnit, UnfinishedBulk)
{
    const size_t bulk_size = 5;
    ControlUnit control_unit;
    for(size_t c = 0; c < bulk_size; c++)
    {
        control_unit.HandleEvent(ControlUnit::Event::CommandAdded);
    }

    ASSERT_EQ(control_unit.GetState(), ControlUnit::State::GatheringStatic);
}


TEST(ControlUnit, DynamicBulkOpened)
{
    const size_t bulk_size = 5;
    ControlUnit control_unit;
    for(size_t c = 0; c < bulk_size - 1; c++)
    {
        control_unit.HandleEvent(ControlUnit::Event::CommandAdded);
    }

    control_unit.HandleEvent(ControlUnit::Event::BlockOpened);

    ASSERT_EQ(control_unit.GetState(), ControlUnit::State::GatheringDynamic);
}

TEST(ControlUnit, GatheringDynamicBulk)
{
    const size_t bulk_size = 5;
    ControlUnit control_unit;
    for(size_t c = 0; c < bulk_size - 1; c++)
    {
        control_unit.HandleEvent(ControlUnit::Event::CommandAdded);
    }

    control_unit.HandleEvent(ControlUnit::Event::BlockOpened);
    control_unit.HandleEvent(ControlUnit::Event::CommandAdded);

    ASSERT_EQ(control_unit.GetState(), ControlUnit::State::GatheringDynamic);
}


TEST(ControlUnit, DynamicBulkClosed)
{
    const size_t bulk_size = 5;
    ControlUnit control_unit;
    for(size_t c = 0; c < bulk_size - 1; c++)
    {
        control_unit.HandleEvent(ControlUnit::Event::CommandAdded);
    }

    control_unit.HandleEvent(ControlUnit::Event::BlockOpened);
    control_unit.HandleEvent(ControlUnit::Event::CommandAdded);
    control_unit.HandleEvent(ControlUnit::Event::CommandAdded);
    control_unit.HandleEvent(ControlUnit::Event::BlockClosed);

    ASSERT_EQ(control_unit.GetState(), ControlUnit::State::DynamicBulkReady);
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}