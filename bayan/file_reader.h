#ifndef FILE_READER_H
#define FILE_READER_H

#include <cstddef>
#include <cstdint>
#include <vector>

class FileReader
{
public:
    virtual size_t max_block_size() = 0;
    virtual bool is_eof() = 0;
    virtual size_t read_block(std::vector<uint8_t>& buffer) = 0;
};

#endif