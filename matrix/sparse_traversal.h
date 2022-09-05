#pragma once

#include <memory>

#include "sparse_storing.h"

using namespace std;

template<typename T, size_t Rank, T default_val>
class tensor_traversal
{
    public:
        tensor_traversal(tensor<T, Rank, default_val>& tensor)
        {
            m_current = tensor.begin();
            m_end = tensor.end();

            if (m_current != m_end)
            {
                m_nested.reset(new tensor_traversal<T, Rank-1, default_val>(m_current->second));
            }
        }

        bool has_values() const
        {
            return m_current != m_end;
        }

        void move_next()
        {
            m_nested->move_next();
            if (!m_nested->has_values())
            {
                ++m_current;
                if (m_current != m_end)
                {
                    m_nested.reset(new tensor_traversal<T, Rank-1, default_val>(m_current->second));
                }
            }
        }

        T get_value() const
        {
            return m_nested->get_value();
        }

        template<size_t L>
        void fill_indicies(array<int, L>& indicies) const
        {
            indicies[L-Rank] = m_current->first;
            m_nested->fill_indicies(indicies);
        }

    private:
        typename map<int, tensor<T, Rank-1, default_val>>::iterator m_current;
        typename map<int, tensor<T, Rank-1, default_val>>::iterator m_end;
        unique_ptr<tensor_traversal<T, Rank-1, default_val>> m_nested; 
};

template<typename T, T default_val>
class tensor_traversal<T, 1, default_val>
{
    public:
        tensor_traversal(tensor<T, 1, default_val> &tensor)
        {
            m_current = tensor.begin();
            m_end = tensor.end();
        }

        bool has_values() const
        {
            return m_current != m_end;
        }

        void move_next()
        {
            ++m_current;
        }

        T get_value() const
        {
            return m_current->second;
        }

        template <size_t L>
        void fill_indicies(array<int, L> &indicies) const
        {
            indicies[L - 1] = m_current->first;
        }

    private:
        typename map<int, T>::iterator m_current;
        typename map<int, T>::iterator m_end;
};