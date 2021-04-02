#ifndef BOOST_DIRECTORY_TRAVERSAL_H
#define BOOST_DIRECTORY_TRAVERSAL_H

#include "directory_traversal.h"

#include <iostream>
#include <boost/filesystem.hpp>
#include <memory>
#include <string>

template <typename IteratorType> 
class BoostDirectoryTraversal : public DirectoryTraversal
{
public:
    BoostDirectoryTraversal(const std::string& path)
    {
        if (!boost::filesystem::exists(path) || !boost::filesystem::is_directory(path))
        {
            throw std::invalid_argument(path + " does not exist or is not a directory.");
        }

        mDirectoryIterator = IteratorType(path);
        mIteratorEnd = IteratorType();
    }
    
    bool is_traversed() override
    {
        return mDirectoryIterator == mIteratorEnd;
    }

    FileComparison get_next_file() override
    {
        std::string path = mDirectoryIterator->path().string();
        do
        {
            mDirectoryIterator++;
        }
        while ((mDirectoryIterator != mIteratorEnd) && boost::filesystem::is_directory(mDirectoryIterator->path().string())); 

        return FileComparison(path, std::shared_ptr<FileReader>(), std::shared_ptr<Hasher>());
    }  

private:
    IteratorType mDirectoryIterator; 
    IteratorType mIteratorEnd;
};

#endif