#include "../include/file_splitter.h"
#include "../include/mapper.h"
#include "../include/min_prefix_functions.h"
#include "../include/reducer.h"
#include "../include/shuffler.h"

#include <atomic>
#include <cstdlib>
#include <condition_variable>
#include <iostream>
#include <fstream>
#include <future>
#include <string>

void run_mapper(
    block_reader * reader,
    const std::function<std::pair<std::string, int>(const std::string&)>& map_func,
    std::multimap<std::string, int>& container,
    std::condition_variable& finish_cv,
    std::atomic_int& counter)
{
    mapper<std::string, int> map_runner(map_func, reader);
    map_runner.run(container);
    --counter;
    finish_cv.notify_one();
}

void run_reducer(
    const std::vector<std::pair<std::string, int>>& container,
    const std::string& out_filename,
    std::condition_variable& finish_cv,
    std::atomic_int& counter)
{
    reducer<max_summator<std::string, int>, std::string, int> reduce_runner(accumulate_key_sum);
    auto result = reduce_runner.run(container);
    std::ofstream outfile(out_filename, std::ios::trunc);
    for (const auto& line: result.get_max_value_str())
    {
        outfile << line << std::endl;
    }
    
    --counter;
    finish_cv.notify_one();
}

int main(int argc, char* argv[])
{
    using namespace std;

    string src;
    int mnum, rnum; 
    if (argc == 1)
    {
        src = "../tests/abc.txt";
        mnum = 10;
        rnum = 5;
    }
    else if (argc != 4)
    {
        cout << "Usage:  mapreduce <src> <mnum> <rnum>" << endl;
        return 0;
    }
    else
    {
        src = argv[1];
        mnum = atoi(argv[2]);
        rnum = atoi(argv[3]);
    }

    auto readers = file_splitter(mnum).split(src);
    vector<multimap<string, int>> after_map(mnum);
    vector<vector<pair<string, int>>> for_reduce(rnum);
    
    mutex mutex;
    condition_variable cv;
    atomic_int threads_to_wait; 

    vector<string> out_filenames(rnum);
    for(auto r = 0; r < rnum; r++)
    {
        out_filenames[r] = "./reducer_" + to_string(r + 1) + ".txt";
    }

    unsigned int prefix_len = 1;
    const int prefix_limit = 200;
    bool full_duplicates = false;
    while(prefix_len < prefix_limit)
    { 
        cout << "Testing prefix length: " + to_string(prefix_len);
        auto map_func = get_prefix_pair_function(prefix_len);

        threads_to_wait = mnum;
        for(auto m = 0; m < mnum; m++)
        {
            auto task = async([m, &readers, &map_func, &after_map, &cv, &threads_to_wait]()
            {
                run_mapper(
                    &readers[m],
                    map_func,
                    std::reference_wrapper<multimap<string, int>>(after_map[m]),
                    reference_wrapper<condition_variable>(cv),
                    reference_wrapper<atomic_int>(threads_to_wait));
            });
        }
        
        unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [&threads_to_wait]{ return threads_to_wait == 0;});

        shuffler<string, int> shaffle;
        shaffle.run(after_map, for_reduce);
        
        threads_to_wait = rnum;
        for(auto r = 0; r < rnum; r++)
        {
            auto task = std::async([r, &for_reduce, &out_filenames, &cv, &threads_to_wait]()
            {
                run_reducer(
                    reference_wrapper<vector<pair<string, int>>>(for_reduce[r]),
                    out_filenames[r],
                    reference_wrapper<condition_variable>(cv),
                    reference_wrapper<atomic_int>(threads_to_wait));
            });
        }

        cv.wait(lock, [&threads_to_wait]{ return threads_to_wait == 0;});

        auto max_duplicates = -1;
        std::string duplicates_key;
        int duplicates;
        for(auto r = 0; r < rnum; r++)
        {
            ifstream r_file(out_filenames[r]);
            r_file >> duplicates;  

            if (duplicates >= 2)
            {
                r_file >> duplicates_key;
                if (duplicates_key.size() < prefix_len)
                {
                    cout << ". Full duplicates found. No solution." << std::endl;
                    cout << "Full duplicates key: " 
                        + duplicates_key + ". Duplicates count: " + to_string(duplicates) << std::endl;
                    full_duplicates = true;
                    break;
                }
            }

            if (duplicates > max_duplicates)
            {
                max_duplicates = duplicates;
            }
        }

        if (full_duplicates)
        {
            break;
        }

        cout << ". Maximum duplicates: " + to_string(max_duplicates) << endl;
        if (max_duplicates < 2)
        {
            break;
        }
        else
        {
            prefix_len++;

            for(auto m = 0; m < mnum; m++)
            {
                after_map[m].clear();
                readers[m].reset();
            }

            for(auto r = 0; r < rnum; r++)
            {
                for_reduce[r].clear();
            }
        }
    }

    if (!full_duplicates)
    {
        cout << "[DONE] Minimum unique prefix length: " + to_string(prefix_len) << endl;
    }

    return 0;
}