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
        cell_proxy(cell_owner<T>* cell_owner, int index)
            :m_cell_owner(cell_owner)
        {
            Rank = N;
            m_indexes[m_dimention++] = index;
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

template<typename T, size_t N, T defval>
class tensor
{
    public:
        T get_value(indexer* indxr)
        {
            int index_pos = indxr->Rank - N; 
            int index = indxr->get_index(index_pos);
            auto iter = m_projection.find(index); 
            if (iter == m_projection.end())
            {
                return defval;
            }
            else
            {
                auto projection = iter->second;
                return projection.get_value(indxr);
            }
        }

        sparse_size_change set_value(T value, indexer* indxr)
        {
            bool is_default = (value == defval);
            
            int index_pos = indxr->Rank - N;
            int index = indxr->get_index(index_pos);
            auto iter = m_projection.find(index);
            bool not_existent = (iter == m_projection.end());  
            
            if (not_existent && is_default)
            {
                return sparse_size_change{0};
            }
            else if (not_existent)
            {
                m_projection[index] = tensor<T, N-1, defval>();
            }

            return m_projection[index].set_value(value, indxr);
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
            int index_pos = indxr->Rank - 1;
            int index = indxr->get_index(index_pos);
            auto iter = m_scalars.find(index); 
            if (iter == m_scalars.end())
            {
                return defval;
            }
            else
            {
                return iter->second;
            }
        }

        sparse_size_change set_value(T value, indexer* indxr)
        {
            bool is_default = (value == defval);

            int index_pos = indxr->Rank - 1;
            int index = indxr->get_index(index_pos);
            auto iter = m_scalars.find(index); 
            bool not_existent = (iter == m_scalars.end());
            
            if (not_existent && is_default)
            {
                return sparse_size_change{0};
            }
            else if (is_default)
            {
                m_scalars.erase(iter);
                return sparse_size_change{-1};
            }
            
            m_scalars[index] = value;

            return not_existent ? sparse_size_change{+1} : sparse_size_change{0};
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