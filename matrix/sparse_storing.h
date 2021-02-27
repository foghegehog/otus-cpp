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
        Leaf get_sparse_value(indexer* indexator) const
        {
            int index = indexator->get_tensor_index(Rank);
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

        sparse_size_change set_sparse_value(Leaf value, indexer* indexator)
        {
            bool is_default = (value == default_val);
            
            int index = indexator->get_tensor_index(Rank);
            auto iter = m_stored.find(index);
            bool not_existent = (iter == m_stored.end());  
            
            if (not_existent && is_default)
            {
                return sparse_size_change{0};
            }

            return set_value_to_stored(indexator, not_existent, is_default, index, value);
        }

    protected:
        map<int, Stored> m_stored;

        virtual Leaf get_value_from_stored(indexer* indexator, const Stored & stored) const = 0;
        virtual sparse_size_change set_value_to_stored(
            indexer* indexator, bool not_existent, bool is_default, int index, Leaf value) = 0; 
};

template<typename T, size_t Rank, T defval>
class tensor: public sparse_level<Rank, tensor<T, Rank-1, defval>, T, defval>
{
    public:
        class cells_traversal
        {
            public:
                cells_traversal(const tensor<T, Rank, defval>& tensor)
                {
                    m_current = tensor.m_projection.begin();
                    m_end = tensor.m_projection.end();

                    if (m_current != m_end)
                    {
                        m_nested = (typename tensor<T, Rank-1, defval>::cells_traversal)(m_current->second);
                    }
                }

                cells_traversal(){}

                bool has_values() const
                {
                    return m_current != m_end;
                }

                void move_next()
                {
                    m_nested.move_next();
                    if (!m_nested.has_values())
                    {
                       ++m_current;
                       if (m_current != m_end)
                       {
                           m_nested = (typename tensor<T, Rank-1, defval>::cells_traversal)(m_current->second);
                       }
                    }
                    m_traversed++;
                }

                T get_value() const
                {
                    return m_nested.get_value();
                }

                template<size_t L>
                void fill_indicies(array<int, L>& indicies) const
                {
                    indicies[L-Rank] = m_current->first;
                    m_nested.fill_indicies(indicies);
                }

                size_t cells_traversed() const
                {
                    return m_traversed;
                }
            private:
                typename map<int, tensor<T, Rank-1, defval>>::const_iterator m_current;
                typename map<int, tensor<T, Rank-1, defval>>::const_iterator m_end;
                typename tensor<T, Rank-1, defval>::cells_traversal m_nested; 
                size_t m_traversed = 0;
        };

    protected:
        T get_value_from_stored(indexer* indexator, const tensor<T, Rank-1, defval>& stored) const override
        {
            return stored.get_sparse_value(indexator);
        }

        sparse_size_change set_value_to_stored(
            indexer* indexator,
            bool not_existent, bool is_default,
            int index, T value) override
        {
            if (not_existent)
            {
                this->m_stored[index] = tensor<T, Rank-1, defval>();
                return this->m_stored[index].set_sparse_value(value, indexator);
            }
            else
            {
                return this->m_stored[index].set_sparse_value(value, indexator);
            } 
        }

    private:
        map<int, tensor<T, Rank-1, defval>> m_projection;

};

template<typename T, T defval>
class tensor<T, 1, defval>: public sparse_level<1, T, T, defval>
{
    public:
        class cells_traversal
        {
            public:
                cells_traversal(const tensor<T, 1, defval>& tensor)
                {
                    m_current = tensor.m_scalars.begin();
                    m_end = tensor.m_scalars.end();
                }

                cells_traversal(){}

                bool has_values() const
                {
                    return m_current != m_end;
                }

                void move_next()
                {
                    ++m_current;
                    m_traversed++;
                }

                T get_value() const
                {
                    return m_current->second;
                }

                template<size_t L>
                void fill_indicies(array<int, L>& indicies) const
                {
                    indicies[L-1] = m_current->first;
                }

                size_t cells_traversed() const
                {
                    return m_traversed;
                }
            private:
                typename map<int, T>::const_iterator m_current;
                typename map<int, T>::const_iterator m_end;
                size_t m_traversed = 0;
        };

    protected:
        T get_value_from_stored(indexer* indexator, const T& stored) const override
        {
            return stored;
        }

        sparse_size_change set_value_to_stored(
            indexer* indexator,
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

    private:
        map<int, T> m_scalars;
};
