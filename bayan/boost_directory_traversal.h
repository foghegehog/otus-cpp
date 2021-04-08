#ifndef BOOST_DIRECTORY_TRAVERSAL_H
#define BOOST_DIRECTORY_TRAVERSAL_H

#include "fstream_file_reader.h"
#include "directory_traversal.h"
#include "scan_depth.h"

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <map>
#include <memory>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

template<typename IteratorType>
bool points_to_directory(const IteratorType& iterator)
{
    return boost::filesystem::is_directory(iterator->path().string());
}

template <typename IteratorType>
class TraversalExcluderBase
{
public:
    TraversalExcluderBase<IteratorType>(
        const std::vector<std::string>& excludeDirectories,
        const std::vector<std::string>& includeFileMask)
        :mExcludeDirectories(excludeDirectories)
    {
        mIncludeFileMask.reserve(includeFileMask.size());  
        for (const auto& globMask: includeFileMask)
        {
            auto replacedMask = globMask;
            for(const auto& pair: mGlobToRegex)
            {
                std::stringstream stream;
                std::ostream_iterator<char> out_iterator(stream);
                boost::regex_replace(out_iterator, replacedMask.begin(), replacedMask.end(), boost::regex(pair.first), pair.second);
                replacedMask = stream.str();
            }

            mIncludeFileMask.push_back(boost::regex("^" + replacedMask + "$"));
        }
    }

    void filter_exclude_dirs(IteratorType& iterator)
    {
        if (std::find(mExcludeDirectories.begin(), mExcludeDirectories.end(), iterator->path().string()) != mExcludeDirectories.end())
        {
            exclude_directory(iterator);
        }
    }

    bool should_include_file(const IteratorType& iterator)
    {
        auto path = iterator->path().string();
        if (boost::filesystem::is_regular_file(path))
        {
            for(const auto& mask: mIncludeFileMask)
            {
                boost::smatch what;
                auto path = iterator->path();
                if (boost::regex_match(path.filename().string(), what, mask))
                {
                    return true;
                }
            }
        }

        return false;
    }

private:
    std::vector<std::string> mExcludeDirectories;
    std::vector<boost::regex> mIncludeFileMask;
    // not using map as the order matters
    const std::vector<std::pair<std::string, std::string>> mGlobToRegex = {{"\\.", "\\\\."}, {"\\*", ".*"}, {"\\?", "."}};

    virtual void exclude_directory(IteratorType& iterator)
    {
        (void)iterator;
    }
};

template <typename IteratorType>
class TraversalExcluder : public TraversalExcluderBase<IteratorType>
{
public:
    TraversalExcluder(const std::vector<std::string>& excludeDirectories, const std::vector<std::string>& includeFileMask)
        :TraversalExcluderBase<IteratorType>(excludeDirectories, includeFileMask)
    {
    }
};

template<>
class TraversalExcluder<boost::filesystem::recursive_directory_iterator> 
    : public TraversalExcluderBase<boost::filesystem::recursive_directory_iterator>
{
public:
    TraversalExcluder(const std::vector<std::string>& excludeDirectories, const std::vector<std::string>& includeFileMask)
        :TraversalExcluderBase(excludeDirectories, includeFileMask)
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
        const std::vector<std::string>& includeFiles,
        std::shared_ptr<Hasher> hasher,
        size_t fileBlocksSize)
        :mDirectories(includeDirs), mFileBlockSize(fileBlocksSize), mHasher(hasher)
    {
        mPathFilter = std::make_unique<TraversalExcluder<IteratorType>>(excludeDirs, includeFiles);
        mDirectoriesIterator = mDirectories.cbegin();
        
        mFilesIterator = mDirectoriesIterator != mDirectories.cend() 
            ? IteratorType(*mDirectoriesIterator)
            : IteratorType();
        mFilesIteratorEnd = IteratorType();

        if (points_to_directory(mFilesIterator) || !mPathFilter->should_include_file(mFilesIterator))
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
            mHasher);
    }  

private:
    std::vector<std::string> mDirectories;
    std::unique_ptr<TraversalExcluder<IteratorType>> mPathFilter; 
    std::vector<std::string>::const_iterator mDirectoriesIterator; 
    IteratorType mFilesIterator; 
    IteratorType mFilesIteratorEnd;
    size_t mFileBlockSize;
    std::shared_ptr<Hasher> mHasher;

    void move_to_next_file()
    {
        do
        {
            if (points_to_directory(mFilesIterator))
            {
                mPathFilter->filter_exclude_dirs(mFilesIterator);
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
        while (!is_traversed() && (points_to_directory(mFilesIterator) || !mPathFilter->should_include_file(mFilesIterator))); 
    }
};

#endif