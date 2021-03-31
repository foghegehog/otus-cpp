#ifndef FILE_COMPARISON_H
#define FILE_COMPARISON_H

#include "file_reader.h"
#include "hasher.h"

#include <cstddef>
#include <list>
#include <memory>
#include <string>
#include <vector>

class FileComparison
{
public:
    FileComparison(
        std::string path,
        std::shared_ptr<FileReader> fileReader,
        std::shared_ptr<Hasher> hasher)
            :mPath(path), mFileReader(std::move(fileReader)), mHasher(std::move(hasher))
    {
    }

    struct Iterator 
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = std::vector<uint8_t>;
        using pointer           = std::vector<uint8_t> *;
        using reference         = std::vector<uint8_t> &;

        Iterator(
            std::list<std::vector<uint8_t>>& hashedBlocks,
            std::shared_ptr<FileReader> fileReader,
            std::shared_ptr<Hasher> hasher)
                :mHashedBlocks(hashedBlocks),
                mBlocksIterator(hashedBlocks.begin()),
                mFileReader(fileReader),
                mHasher(hasher)
        {
            mBuffer.reserve(mFileReader->max_block_size());
        } 

        reference operator*();
        pointer operator->();
        Iterator& operator++(); 
        Iterator operator++(int);

        friend bool operator== (const Iterator& a, const Iterator& b); 
        friend bool operator!= (const Iterator& a, const Iterator& b);

    private:
        std::list<std::vector<uint8_t>>& mHashedBlocks; 
        std::list<std::vector<uint8_t>>::iterator mBlocksIterator; 
        std::shared_ptr<FileReader> mFileReader;
        std::vector<uint8_t> mBuffer;
        std::shared_ptr<Hasher> mHasher;     
    };

    Iterator begin() { return Iterator(mHashedBlocks, mFileReader, mHasher); }
    Iterator end()   { return Iterator(mHashedBlocks, std::shared_ptr<FileReader>(nullptr), mHasher); }

    std::string mPath;
    size_t mFileSize;
    // list is preffered over vector as its iterators are not invalidated upon insertion
    std::list<std::vector<uint8_t>> mHashedBlocks;
    
    std::list<std::string> mDuplicatePaths;

private:
    std::shared_ptr<FileReader> mFileReader;
    std::shared_ptr<Hasher> mHasher;

    
};

#endif