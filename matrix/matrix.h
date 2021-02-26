#pragma once

#include <array>
#include <map>
#include "tuple_repeat.h"

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

        class cells_traversal
        {
            public:
                cells_traversal(const tensor<T, N, defval>& tensor)
                {
                    m_current = tensor.m_projection.begin();
                    m_end = tensor.m_projection.end();

                    if (m_current != m_end)
                    {
                        m_nested = (typename tensor<T, N-1, defval>::cells_traversal)(m_current->second);
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
                           m_nested = (typename tensor<T, N-1, defval>::cells_traversal)(m_current->second);
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
                    indicies[L-N] = m_current->first;
                    m_nested.fill_indicies(indicies);
                }

                size_t cells_traversed() const
                {
                    return m_traversed;
                }
            private:
                typename map<int, tensor<T, N-1, defval>>::const_iterator m_current;
                typename map<int, tensor<T, N-1, defval>>::const_iterator m_end;
                typename tensor<T, N-1, defval>::cells_traversal m_nested; 
                size_t m_traversed = 0;
        };

        void traverse(array<int, N>& indicies)
        {
            auto traversal = cells_traversal(*this);
            traversal.move_next();
            traversal.fill_indicies(indicies);
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

    private:
        map<int, T> m_scalars;
};

template<typename T, size_t N, T defval>
class Matrix : cell_owner<T>
{
    public:
        using cell_type = typename tuple_repeat<T, N, int>::tuple_type;

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

        struct Iterator 
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = cell_type;
            using pointer           = cell_type*;
            using reference         = cell_type&;

            Iterator(const tensor<T, N, defval>& tensor)
            {
                m_traversal = (typename tensor<T, N, defval>::cells_traversal)(tensor);
            }

            value_type operator*() const 
            { 
                auto value = m_traversal.get_value(); 
                array<int, N> indicies;
                m_traversal.fill_indicies(indicies);
                return create_tuple_repeat(indicies, value);
            }
            value_type operator->() 
            {
                auto value = m_traversal.get_value(); 
                array<int, N> indicies;
                m_traversal.fill_indicies(indicies);
                return create_tuple_repeat(indicies, value); 
            }
            Iterator& operator++() 
            {
                m_traversal.move_next(); 
                return *this; 
            }  

            Iterator operator++(int)
            {
                Iterator tmp = *this;
                ++(*this);
                return tmp; 
            }

            friend bool operator== (const Iterator& a, const Iterator& b) 
            {
                return a.m_traversal.has_values() == b.m_traversal.has_values(); 
            };

            friend bool operator!= (const Iterator& a, const Iterator& b)
            {
                return a.m_traversal.has_values() != b.m_traversal.has_values(); 
            };  



        private:
            typename tensor<T, N, defval>::cells_traversal m_traversal;
        };

        Iterator begin() { return Iterator(m_tensor); }
        Iterator end()   { return Iterator(m_empty_tensor); }
        /*void traverse(array<int, N>& indicies)
        {
            m_tensor.traverse(indicies);
        }*/

    private:
        tensor<T, N, defval> m_tensor;
        size_t m_size = 0;

        tensor<T, N, defval> m_empty_tensor;

};