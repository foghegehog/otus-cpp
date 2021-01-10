#include <gtest/gtest.h>

TEST(TestInput, Split)
{
    ASSERT_GT(0, 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

