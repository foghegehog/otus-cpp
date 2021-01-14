#include "string_lib.h"
#include <gtest/gtest.h>

// ("",  '.') -> [""]
TEST(CheckSplit, EmptyString)
{
    auto result = split("", '.');
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0], "");
}

// ("11", '.') -> ["11"]
TEST(CheckSplit, SingleString)
{
    auto result = split("11", '.');
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0], "11");
}

// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
TEST(CheckSplit, TwoEmptyParts)
{
    auto result = split("..", '.');
    ASSERT_EQ(result.size(), 3);
    for (auto const &part: result)
    {
        ASSERT_EQ(part, "");
    }
}

// ("11.", '.') -> ["11", ""]
TEST(CheckSplit, NonEmptyEmptyParts)
{
    auto result = split("11.", '.');
    ASSERT_EQ(result.size(), 2);
    ASSERT_EQ(result[0], "11");
    ASSERT_EQ(result[1], "");
}

// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
TEST(CheckSplit, EmptyNonEmptyParts)
{
    auto result = split(".11", '.');
    ASSERT_EQ(result.size(), 2);
    ASSERT_EQ(result[0], "");
    ASSERT_EQ(result[1], "11");
}

// ("11.22", '.') -> ["11", "22"]
TEST(CheckSplit, TwoNonEmptyParts)
{
    auto result = split("11.22", '.');
    ASSERT_EQ(result.size(), 2);
    ASSERT_EQ(result[0], "11");
    ASSERT_EQ(result[1], "22");
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

