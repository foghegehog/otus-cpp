#pragma once

#include "interfaces.h"

#include <array>
#include <map>

using namespace std;

struct sparse_size_change
{
    int change;
}; 

template<size_t Rank, typename Stored, typename Leaf, Leaf default_val>
class sparse_level
{
    public:
        Leaf get_sparse_value(const indexer& indexator) const
        {
            int index = indexator.get_tensor_index(Rank);
            auto iter = m_stored.find(index); 
            if (iter == m_stored.end())
            {
                return default_val;
            }
            else
            {
                return get_value_from_stored(indexator, iter->second);
            }
        }

        sparse_size_change set_sparse_value(Leaf value, const indexer& indexator)
        {
            bool is_default = (value == default_val);
            
            int index = indexator.get_tensor_index(Rank);
            auto iter = m_stored.find(index);
            bool not_existent = (iter == m_stored.end());  
            
            if (not_existent && is_default)
            {
                return sparse_size_change{0};
            }

            return set_value_to_stored(indexator, not_existent, is_default, index, value);
        }

        bool empty()
        {
            return m_stored.empty();
        }

        typename map<int, Stored>::iterator begin()
        {
            return m_stored.begin();
        }

        typename map<int, Stored>::iterator end()
        {
            return m_stored.end();
        }

    protected:
        map<int, Stored> m_stored;

        virtual Leaf get_value_from_stored(const indexer& indexator, const Stored & stored) const = 0;
        virtual sparse_size_change set_value_to_stored(
            const indexer& indexator, bool not_existent, bool is_default, int index, Leaf value) = 0; 
};

template<typename T, size_t Rank, T defval>
class tensor: public sparse_level<Rank, tensor<T, Rank-1, defval>, T, defval>
{
    protected:
        T get_value_from_stored(const indexer& indexator, const tensor<T, Rank-1, defval>& stored) const override
        {
            return stored.get_sparse_value(indexator);
        }

        sparse_size_change set_value_to_stored(
            const indexer& indexator,
            bool not_existent, bool is_default,
            int index, T value) override
        {
            if (not_existent)
            {
                this->m_stored[index] = tensor<T, Rank-1, defval>();
            }
            
            auto& stored = this->m_stored[index];
            auto size_change = stored.set_sparse_value(value, indexator);
            if (stored.empty())
            {
                this->m_stored.erase(index);
            }
            return size_change;
        }
};

template<typename T, T defval>
class tensor<T, 1, defval>: public sparse_level<1, T, T, defval>
{
    protected:
        T get_value_from_stored(const indexer& indexator, const T& stored) const override
        {
            return stored;
        }

        sparse_size_change set_value_to_stored(
            const indexer& indexator,
            bool not_existent, bool is_default,
            int index, T value) override
        {
            if (not_existent)
            {
                this->m_stored[index] = value;
                return sparse_size_change{+1};
            }
            else if (is_default)
            {
                this->m_stored.erase(index);
                return sparse_size_change{-1};
            }
            else
            {
                this->m_stored[index] = value;
                return sparse_size_change{0};
            }
        }
};
