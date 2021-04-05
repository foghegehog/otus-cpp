#ifndef BOOST_DIRECTORY_TRAVERSAL_H
#define BOOST_DIRECTORY_TRAVERSAL_H

#include "fstream_file_reader.h"
#include "directory_traversal.h"
#include "scan_depth.h"

#include <iostream>
#include <boost/filesystem.hpp>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

template<typename IteratorType>
bool points_to_directory(const IteratorType& iterator)
{
    return boost::filesystem::is_directory(iterator->path().string());
}

template <typename IteratorType>
class DirectoryExcluderBase
{
public:
    DirectoryExcluderBase<IteratorType>(const std::vector<std::string>& excludeDirectories)
        :mExcludeDirectories(excludeDirectories)
    {
    }

    void filter_exclude_dirs(IteratorType& iterator)
    {
        if (points_to_directory(iterator) && 
                std::find(mExcludeDirectories.begin(), mExcludeDirectories.end(), iterator->path().string()) != mExcludeDirectories.end())
        {
            exclude_directory(iterator);
        }
    }

private:
    std::vector<std::string> mExcludeDirectories;
    virtual void exclude_directory(IteratorType& iterator)
    {
        (void)iterator;
    }
};

template <typename IteratorType>
class DirectoryExcluder : public DirectoryExcluderBase<IteratorType>
{
public:
    DirectoryExcluder(const std::vector<std::string>& excludeDirectories)
        :DirectoryExcluderBase<IteratorType>(excludeDirectories)
    {
    }
};

template<>
class DirectoryExcluder<boost::filesystem::recursive_directory_iterator> 
    : public DirectoryExcluderBase<boost::filesystem::recursive_directory_iterator>
{
public:
    DirectoryExcluder(const std::vector<std::string>& excludeDirectories)
        :DirectoryExcluderBase(excludeDirectories)
    {
    }

private:
    void exclude_directory(boost::filesystem::recursive_directory_iterator& iterator)
    {
        iterator.disable_recursion_pending();
    }
};

template <typename IteratorType> 
class BoostDirectoryTraversal : public DirectoryTraversal
{
public:
    BoostDirectoryTraversal(
        const std::vector<std::string>& includeDirs,
        const std::vector<std::string>& excludeDirs,
        size_t fileBlocksSize)
        :mDirectories(includeDirs), mFileBlockSize(fileBlocksSize)
    {
        mDirectoryExcluder = std::make_unique<DirectoryExcluder<IteratorType>>(excludeDirs);
        mDirectoriesIterator = mDirectories.cbegin();
        
        mFilesIterator = mDirectoriesIterator != mDirectories.cend() 
            ? IteratorType(*mDirectoriesIterator)
            : IteratorType();
        mFilesIteratorEnd = IteratorType();

        if (points_to_directory(mFilesIterator))
        {
            move_to_next_file();
        }
    }
    
    bool is_traversed() const override
    {
        return (mFilesIterator == mFilesIteratorEnd) && (mDirectoriesIterator == mDirectories.cend()); 
    }

    FileComparison get_next_file() override
    {
        std::string path = mFilesIterator->path().string();

        move_to_next_file();

        return FileComparison(
            path, 
            boost::filesystem::file_size(path),
            move(std::make_shared<FstreamFileReader>(path, mFileBlockSize)),
            move(std::make_shared<Hasher>()));
    }  

private:
    std::vector<std::string> mDirectories;
    std::unique_ptr<DirectoryExcluder<IteratorType>> mDirectoryExcluder; 
    std::vector<std::string>::const_iterator mDirectoriesIterator; 
    IteratorType mFilesIterator; 
    IteratorType mFilesIteratorEnd;
    size_t mFileBlockSize;

    void move_to_next_file()
    {
        do
        {
            if (points_to_directory(mFilesIterator))
            {
                mDirectoryExcluder->filter_exclude_dirs(mFilesIterator);
            }

            ++mFilesIterator;

            if (mFilesIterator == mFilesIteratorEnd)
            {
                ++mDirectoriesIterator;
                if (mDirectoriesIterator != mDirectories.cend())
                {
                    mFilesIterator = IteratorType(*mDirectoriesIterator);
                }
            }
        }
        while (!is_traversed() && points_to_directory(mFilesIterator)); 
    }
};

#endif