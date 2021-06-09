#include "../include/block_reader.h"
#include "../include/file_splitter.h"
#include "../include/max_summator.h"
#include "../include/mapper.h"
#include "../include/min_prefix_functions.h"
#include "../include/shuffler.h"
#include "../include/reducer.h"

#include <gtest/gtest.h>
#include <iostream>
#include <string>

TEST(Framework, Shuffle)
{
    std::vector<std::vector<std::pair<std::string, int>>> after_map;
    size_t repeat = 5;
    for (size_t i = 0; i < repeat; i++)
    {
        std::vector<std::pair<std::string, int>> map =
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
    auto max = result.get_max_value();
    ASSERT_EQ(max, 15);
}

class block_reader_mock : public block_reader
{
public:
    block_reader_mock(std::vector<std::string>::iterator from, std::vector<std::string>::iterator to)
    {
        m_lines.insert(m_lines.end(), from, to);
        m_pos = m_lines.begin();
    }

    bool get_next_line(std::string& line) override
    {
        if (m_pos != m_lines.end())
        {
            line = *m_pos;
            ++m_pos;
            return true;
        }
        else
        {
            return false;
        }
    }

private:
    std::vector<std::string> m_lines; 
    std::vector<std::string>::iterator m_pos;
};

TEST(Framework, Usage)
{
    std::vector<std::string> lines = 
    {
        "a",
        "bb",
        "ccccc",
        "aa",
        "aaa",
        "b",
        "c",
        "ddddd",
        "aaaa",
        "eee",
        "fff",
        "ggg",
        "aaaan",
        "aaaaaaaa",
        "aaaaaaaab",
    };

    size_t prefix_len;
    for (prefix_len = 1; prefix_len < 1000; ++prefix_len)
    {
        auto map_func = get_prefix_pair_function(prefix_len);

        const int mappers_count = 5;
        std::vector<std::vector<std::pair<std::string, int>>> after_map(mappers_count);
        int block_size = lines.size() / mappers_count;
        for(auto m = 0; m < mappers_count; m++)
        {
            auto reader = block_reader_mock(lines.begin() + m * block_size, lines.begin() + (m +1) * block_size);
            mapper<std::string, int> map_runner(map_func, &reader);
            map_runner.run(after_map[m]);
        }

        const int reducers_count = 3;
        std::vector<std::vector<std::pair<std::string, int>>> for_reduce(reducers_count);

        shuffler<std::string, int> shaffle;
        shaffle.run(after_map, for_reduce);
        
        auto max_duplicates = 0;
        for(auto r = 0; r < reducers_count; r++)
        {
            reducer<max_summator<std::string, int>, std::string, int> reduce_runner(accumulate_key_sum);
            auto result = reduce_runner.run(for_reduce[r]);
            auto duplicates = result.get_max_value();
            if (duplicates > max_duplicates)
            {
                max_duplicates = duplicates;
            }
        }

        if (max_duplicates == 1)
        {
            break;
        }
    }

    ASSERT_EQ(prefix_len, lines.back().size());
}

/*TEST(FileOperationss, BlocksDivision)
{
    const int lines_count = 23;
    file_splitter splitter(lines_count);
    auto readers = splitter.split("include/file_splitter.h");
    
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
            //std::cout << line << std::endl;
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
}*/

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}