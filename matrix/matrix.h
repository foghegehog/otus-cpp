#pragma once

#include "cell_proxy.h"
#include "interfaces.h"
#include "sparse_storing.h"
#include "tuple_repeat.h"

#include <array>
#include <map>

using namespace std;


template<typename T, size_t N, T defval>
class Matrix : cell_owner<T>
{
    public:
        using cell_type = typename tuple_repeat<T, N, int>::tuple_type;

        cell_proxy<T, N> operator[](int index)
        {
            return cell_proxy<T, N>(this, index);
        }

        T get_cell_value(indexer* indexator) override
        {
            return m_tensor.get_sparse_value(indexator);
        }

        void set_cell_value(T value, indexer* indexator) override
        {
            auto size_change = m_tensor.set_sparse_value(value, indexator);
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

    private:
        tensor<T, N, defval> m_tensor;
        size_t m_size = 0;

        tensor<T, N, defval> m_empty_tensor;

};