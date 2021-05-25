#include "../include/table.h"

#include <gtest/gtest.h>
#include <string>

TEST(TableOperations, Insert)
{
    Table A;
    bool different_succes = true;
    const int num_records = 6;
    for (auto id = 0; id < num_records; id++)
    {
        different_succes &= A.Insert(id, std::to_string(id));
    }

    auto dublicate_success = A.Insert(0, "lean");

    ASSERT_TRUE(different_succes);
    ASSERT_FALSE(dublicate_success);
}

TEST(TableOperations, Intersection)
{
    Table A, B;
    A.Insert(0, "lean");
    A.Insert(1, "sweater");
    A.Insert(2, "frank");
    A.Insert(3, "violation");
    A.Insert(4, "quality");
    A.Insert(5, "precision");

    B.Insert(3, "proposal");
    B.Insert(4, "example");
    B.Insert(5, "lake");
    B.Insert(6, "flour");
    B.Insert(7, "wonder");
    B.Insert(8, "selection");

    auto result = Intersect(A, B);
    A.Truncate();
    B.Truncate();

    ASSERT_EQ(result.m_records.size(), 3);
    for(auto i = 0, id = 3; id <=5 ; i++, id++)
    {
        ASSERT_EQ(result.m_records[i].m_left->id, id);
        ASSERT_EQ(result.m_records[i].m_right->id, id);
    }
}

TEST(TableOperations, Truncate)
{
    Table A;
    A.Insert(0, "lean");
    A.Insert(1, "sweater");
    A.Insert(2, "frank");
    A.Insert(3, "violation");
    A.Insert(4, "quality");
    A.Insert(5, "precision");

    A.Truncate();
    auto result = Intersect(A, A);
    ASSERT_EQ(result.m_records.size(), 0);
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}