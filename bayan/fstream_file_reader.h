#ifndef BOOST_FILE_READER_H
#define BOOST_FILE_READER_H

#include <fstream>
#include <string>

#include "file_reader.h"

class FstreamFileReader : public FileReader
{
public:
    FstreamFileReader(std::string path, size_t block_size)
        :mFileStream(path), mBlockSize(block_size)
    {
    }

    size_t max_block_size() const override
    {
        return mBlockSize;
    }

    bool is_eof() const override
    {
        return mFileStream.eof();
    }

    size_t read_block(std::vector<uint8_t>& buffer) override
    {
        mFileStream.read((char*)buffer.data(), mBlockSize);
        return (size_t)mFileStream.gcount();
    }

private:
    std::ifstream mFileStream;
    size_t mBlockSize; 
};

#endif