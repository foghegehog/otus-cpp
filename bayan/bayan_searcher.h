#ifndef BAYAN_SEARCHER_H
#define BAYAN_SEARCHER_H

#include "directory_traversal.h"
#include "file_reader.h"
#include "hasher.h"

#include <memory>

class BayanSearcher
{

public:
    BayanSearcher(
        std::shared_ptr<DirectoryTraversal> directory_traversal,
        std::shared_ptr<Hasher> hasher)
        :mDirectoryTraversal(directory_traversal), mHasher(hasher)
    {
    }    

private:
    std::shared_ptr<DirectoryTraversal> mDirectoryTraversal;
    std::shared_ptr<Hasher> mHasher;
};

#endif