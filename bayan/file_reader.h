#ifndef FILE_READER_H
#define FILE_READER_H

#include <cstddef>
#include <cstdint>

class FileReader
{
    virtual size_t read_block(uint8_t * buffer, size_t max_length) = 0;
};

#endif