#pragma once

#include <array>
#include <map>

using namespace std;

class indexer
{
    public:
        size_t Rank;
        virtual int get_index(int dimention) = 0;
};

template<typename T>
class cell_owner
{
    public:
        virtual T get_cell_value(indexer* indxr) = 0;
        virtual void set_cell_value(T value, indexer* indxr) = 0;
};

template<typename T, size_t N>
class cell_proxy : public indexer 
{
    public:
        cell_proxy(cell_owner<T>* cell_owner, int first_index)
            :m_cell_owner(cell_owner)
        {
            Rank = N;
            m_indexes[m_dimention++] = first_index;
        }

        cell_proxy& operator[](int index)
        {
            m_indexes[m_dimention++] = index;
            return *this;
        }

        int get_index(int dimention) override
        {
            if (dimention >= N)
            {
                throw out_of_range(to_string(dimention));
            }

            return m_indexes[dimention];
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
        array<int, N> m_indexes;
        int m_dimention = 0;
};

struct sparse_size_change
{
    int change;
}; 

template<typename Stored, typename T, T defval>
class sparse_searcher
{
    public:
        static T get_value(indexer* indxr, size_t N, map<int, Stored> stored)
        {
            int index_pos = indxr->Rank - N; 
            int index = indxr->get_index(index_pos);
            auto iter = stored.find(index); 
            if (iter == stored.end())
            {
                return defval;
            }
            else
            {
                return get_value(indxr, iter->second);
            }
        }

        static sparse_size_change set_value(T value, indexer* indxr, size_t N, map<int, Stored>& stored)
        {
            bool is_default = (value == defval);
            
            int index_pos = indxr->Rank - N;
            int index = indxr->get_index(index_pos);
            auto iter = stored.find(index);
            bool not_existent = (iter == stored.end());  
            
            if (not_existent && is_default)
            {
                return sparse_size_change{0};
            }

            return handle_values_stored(indxr, not_existent, is_default, stored, index, value);
        }

    private:
        static T get_value(indexer* indxr, T leaf) 
        {
            return leaf;
        }

        template<typename Inner>
        static T get_value(indexer* indxr, Inner& node) 
        {
            return node.get_value(indxr);
        }

        template<typename Inner>
        static sparse_size_change handle_values_stored(
            indexer* indxr,
            bool not_existent, bool is_default,
            map<int, Inner>& stored,
            int index, T value)
        {
            if (not_existent)
            {
                stored[index] = Inner();
                return stored[index].set_value(value, indxr);
            }
            else
            {
                return stored[index].set_value(value, indxr);
            } 
        }

        static sparse_size_change handle_values_stored(
            indexer* indxr,
            bool not_existent, bool is_default,
            map<int, T>& stored,
            int index,  T value)
        {
            if (not_existent)
            {
                stored[index] = value;
                return sparse_size_change{+1};
            }
            else if (is_default)
            {
                stored.erase(index);
                return sparse_size_change{-1};
            }
            else
            {
                stored[index] = value;
                return sparse_size_change{0};
            } 
        }

};

template<typename T, size_t N, T defval>
class tensor
{
    public:
        T get_value(indexer* indxr) 
        {
            return sparse_searcher<tensor<T, N-1, defval>, T, defval>::get_value(indxr, N, m_projection);
        }

        sparse_size_change set_value(T value, indexer* indxr)
        {
            return sparse_searcher<tensor<T, N-1, defval>, T, defval>::set_value(value, indxr, N, m_projection);
        }

    private:
        map<int, tensor<T, N-1, defval>> m_projection;

};

template<typename T, T defval>
class tensor<T, 1, defval>
{
    public:
        T get_value(indexer* indxr)
        {
            return sparse_searcher<T, T, defval>::get_value(indxr, 1, m_scalars);
        }

        sparse_size_change set_value(T value, indexer* indxr)
        {
            return sparse_searcher<T, T, defval>::set_value(value, indxr, 1, m_scalars);
        }

    private:
        map<int, T> m_scalars;
};

template<typename T, size_t N, T defval>
class Matrix : cell_owner<T>
{
    public:
        cell_proxy<T, N> operator[](int index)
        {
            return cell_proxy<T, N>(this, index);
        }

        T get_cell_value(indexer* indxr) override
        {
            return m_tensor.get_value(indxr);
        }

        void set_cell_value(T value, indexer* indxr) override
        {
            auto size_change = m_tensor.set_value(value, indxr);
            m_size += size_change.change;
        }

        size_t size()
        {
            return m_size;
        }

    private:
        tensor<T, N, defval> m_tensor;
        size_t m_size = 0;
};