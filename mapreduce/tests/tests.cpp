#include "../include/file_splitter.h"
#include "../include/shuffler.h"
#include "../include/reducer.h"
#include "../include/max_summator.h"

#include <gtest/gtest.h>
#include <iostream>
#include <string>

TEST(Framework, Shuffle)
{
    std::vector<std::multimap<std::string, int>> after_map;
    size_t repeat = 5;
    for (size_t i = 0; i < repeat; i++)
    {
        std::multimap<std::string, int> map =
        {
            std::make_pair<std::string, int>("aaa", i),
            std::make_pair<std::string, int>("aaa", i + 1),
            std::make_pair<std::string, int>("bbb", i),
            std::make_pair<std::string, int>("ccc", i),
            std::make_pair<std::string, int>("ddd", i),
            std::make_pair<std::string, int>("ddd", i + 1),
            std::make_pair<std::string, int>("eee", i),
            std::make_pair<std::string, int>("eee", i + 1),
            std::make_pair<std::string, int>("fffff", i),
            std::make_pair<std::string, int>("gggggg", i)
        };
        after_map.push_back(map);
    }
     

    std::vector<std::vector<std::pair<std::string, int>>> for_reduce(5);
    shuffler<std::string, int> sh;
    sh.run(after_map, for_reduce);

    for (const auto& reducer: for_reduce)
    {
        ASSERT_EQ(reducer.size(), repeat * 2);
    }

    auto a_reducer = for_reduce[0];
    for(const auto& pair: a_reducer)
    {
        auto key = pair.first;
        ASSERT_EQ(key, "aaa");
    }   

    auto bc_reducer = for_reduce[1];
    for(const auto& pair: bc_reducer)
    {
        auto key = pair.first;
        ASSERT_TRUE((key == "bbb") || (key == "ccc"));
    } 

    auto d_reducer = for_reduce[2];
    for(const auto& pair: d_reducer)
    {
        auto key = pair.first;
        ASSERT_EQ(key, "ddd");
    } 

    auto e_reducer = for_reduce[3];
    for(const auto& pair: e_reducer)
    {
        auto key = pair.first;
        ASSERT_EQ(key, "eee");
    } 

    auto fg_reducer = for_reduce[4];
    for(const auto& pair: fg_reducer)
    {
        auto key = pair.first;
        ASSERT_TRUE((key == "fffff") || (key == "gggggg"));
    } 
    
}

TEST(Framework, Reduce)
{
    auto func = [](max_summator<std::string, int> & accum, std::pair<std::string, int> pair)
    {
        accum.add(pair);
    };

    reducer<max_summator<std::string, int>, std::string, int> r(func);

    std::vector<std::pair<std::string, int>> pairs =
    {
        std::make_pair<std::string, int>("aaa", 1),
        std::make_pair<std::string, int>("aaa", 2),
        std::make_pair<std::string, int>("bbb", 3),
        std::make_pair<std::string, int>("ccc", 4),
        std::make_pair<std::string, int>("ddd", 5),
        std::make_pair<std::string, int>("ddd", 6),
        std::make_pair<std::string, int>("eee", 7),
        std::make_pair<std::string, int>("eee", 8), 
        std::make_pair<std::string, int>("fffff", 9),
        std::make_pair<std::string, int>("gggggg", 10)
    };

    auto result = r.run(pairs);
    auto pair = result.get_max_pair();
    ASSERT_EQ(pair.first, "eee");
    ASSERT_EQ(pair.second, 15);
}

TEST(FileOperationss, BlocksDivision)
{
    const int lines_count = 64;
    file_splitter<lines_count> splitter;
    auto readers = splitter.split("../include/file_splitter.h");
    
    std::string first_line;
    std::string last_line;
    int empty_readers = 0;

    auto readers_it = readers.begin();
    auto first_reader_ok = readers_it->get_next_line(first_line);
    std::string line;
    for(++readers_it; readers_it != readers.end(); ++readers_it)
    {
        bool no_reads = true;
        while(readers_it->get_next_line(line))
        {
            no_reads = false;
            last_line = line.empty() ? last_line : line;
        }

        if (no_reads)
        {
            ++empty_readers;
        }
    }

    ASSERT_TRUE(first_reader_ok);
    ASSERT_GT(empty_readers, 0);
    ASSERT_LT(empty_readers, readers.size());
    ASSERT_EQ(first_line, "#ifndef FILE_SPLITTER_H");
    ASSERT_EQ(last_line, "#endif");
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}