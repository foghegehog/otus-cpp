#include "file_comparison.h"

using namespace std;

std::vector<uint8_t>& FileComparison::Iterator::operator*()
{
    if (mBlocksIterator != mHashedBlocks.end())
    {
        return *mBlocksIterator;
    }
    else if (mFileReader && mFileReader->is_eof())
    {
        auto read_cont = mFileReader->read_block(mBuffer);
        auto pad_count = mFileReader->max_block_size() - read_cont;
        for (size_t i = 0; i < pad_count; i++)
        {
            mBuffer.push_back(static_cast<uint8_t>(0));
        }

        auto hashed_block = mHasher->hash_block(mBuffer);
        mBlocksIterator = mHashedBlocks.insert(mBlocksIterator, hashed_block);
    }
    else
    {
        throw std::range_error("Attempt to read iterator that has reached end.");
    }

    return *mBlocksIterator;
}

std::vector<uint8_t>* FileComparison::Iterator::operator->()
{
    return &(*mBlocksIterator);
}

FileComparison::Iterator& FileComparison::Iterator::operator++() 
{
    mBlocksIterator++;
    return *this; 
}

FileComparison::Iterator FileComparison::Iterator::operator++(int) 
{
    Iterator tmp = *this;
    ++(*this);
    return tmp; 
}

bool operator==(
    const FileComparison::Iterator& a,
    const FileComparison::Iterator& b)
{
    if (a.mBlocksIterator != b.mBlocksIterator)
    {
        return false;
    }
    
    return (a.mFileReader && !a.mFileReader->is_eof()) == (b.mFileReader && !b.mFileReader->is_eof()); 
} 

bool operator!=(
    const FileComparison::Iterator& a,
    const FileComparison::Iterator& b)
{
    if (a.mBlocksIterator != b.mBlocksIterator)
    {
        return true;
    }

    return (a.mFileReader && !a.mFileReader->is_eof()) != (b.mFileReader && !b.mFileReader->is_eof());
}