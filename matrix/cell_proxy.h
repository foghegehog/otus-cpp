#pragma once

#include "interfaces.h"

#include <array>
#include <cstddef>

using namespace std;

template<typename T, size_t Rank>
class cell_proxy : public indexer 
{
    public:
        cell_proxy(cell_owner<T>* cell_owner, int first_index)
            :m_cell_owner(cell_owner)
        {
            m_indexes[m_dimention++] = first_index;
        }

        cell_proxy& operator[](int index)
        {
            m_indexes[m_dimention++] = index;
            return *this;
        }

        int get_tensor_index(int rank) override
        {
            if ((rank > Rank)|| (rank<0))
            {
                throw out_of_range(to_string(rank));
            }

            return m_indexes[Rank-rank];
        }

        operator T() 
        {
            return m_cell_owner->get_cell_value(this);
        }

        cell_proxy& operator=(const T& other) 
        {
            m_cell_owner->set_cell_value(other, this);
            return *this;
        };

    private:
        cell_owner<T>* m_cell_owner;
        array<int, Rank> m_indexes;
        int m_dimention = 0;
};