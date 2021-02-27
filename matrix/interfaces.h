#pragma once

#include <cstddef>

class indexer
{
    public:
        // returns index if the submatrix with specified rank (i.e. its number of dimentions) 
        virtual int get_tensor_index(int rank) = 0;
};

template<typename T>
class cell_owner
{
    public:
        virtual T get_cell_value(indexer* indxr) = 0;
        virtual void set_cell_value(T value, indexer* indxr) = 0;
};