#ifndef FILE_SPLITTER_H
#define FILE_SPLITTER_H

#include "block_reader.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

template<size_t N>
class file_splitter
{
public:
    std::vector<file_block_reader> split(const std::string& path);
};


template<size_t N>
std::vector<file_block_reader> file_splitter<N>::split(const std::string& path)
{    
    std::vector<file_block_reader> readers;
    readers.reserve(N);

    std::filesystem::directory_entry file_info(path);
    auto block_start = 0;
    auto rest_size = file_info.file_size();

    if (N == 1)
    {
        readers.emplace_back(path, block_start, rest_size);
        return readers; 
    }

    const char linebreak = '\n';
    char symbol;
    std::ifstream file_content(path, std::fstream::binary);
    for(size_t reader_i = 0; reader_i < N; ++reader_i)
    {
        auto rest_parts = N - reader_i;
        auto block_length = rest_size / rest_parts;
        file_content.seekg(block_length, file_content.cur);
        
        do
        {
            file_content.get(symbol);
            if (!file_content)
            {
                break;
            }

            ++block_length;
            --rest_size;
        } 
        while (symbol != linebreak);

        readers.emplace_back(path, block_start, block_length);
        block_start += block_length;
    }

    return readers;
}

#endif