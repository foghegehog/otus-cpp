#ifndef BAYAN_SEARCHER_H
#define BAYAN_SEARCHER_H

#include "directory_traversal.h"
#include "file_comparison.h"

#include <memory>
#include <vector>

class BayanSearcher
{

public:
    BayanSearcher(
        std::unique_ptr<DirectoryTraversal> directory_traversal)
        :mDirectoryTraversal(move(directory_traversal))
    {
    }    

    void search_bayans();

    std::vector<FileComparison> mComparisonFiles; 

private:
    std::unique_ptr<DirectoryTraversal> mDirectoryTraversal;
};

#endif