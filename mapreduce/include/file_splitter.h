#ifndef FILE_SPLITTER_H
#define FILE_SPLITTER_H

#include "block_reader.h"

#include <fstream>
#include <string>
#include <vector>

class file_splitter
{
public:
    file_splitter(size_t blocks)
        :m_blocks_count(blocks)
    {}

    std::vector<file_block_reader> split(const std::string& path);
private:
    size_t m_blocks_count;
};

#endif