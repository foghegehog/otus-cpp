#ifndef DIRECTORY_TRAVERSAL_H
#define DIRECTORY_TRAVERSAL_H

#include "file_comparison.h"

#include <string>

class DirectoryTraversal
{
public:
    virtual bool is_traversed() = 0;
    virtual FileComparison get_next_file() = 0;  
};

#endif