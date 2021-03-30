#ifndef DIRECTORY_TRAVERSAL_H
#define DIRECTORY_TRAVERSAL_H

#include <string>

class DirectoryTraversal
{
    virtual bool is_traversed() = 0;
    virtual std::string get_next_file() = 0;  
};

#endif