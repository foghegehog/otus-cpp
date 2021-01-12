#include <gtest/gtest.h>

TEST(CheckGTestInstalled, AlwaysTrue)
{
    ASSERT_GT(2, 1);
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

