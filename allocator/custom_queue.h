#pragma once

#include <cstdlib>
#include <stdexcept> 

template<typename T, size_t S, typename allocator>
class custom_queue
{
    public:
        custom_queue()
            : m_allocator(allocator()), m_front_index(0), m_back_index(0)
        {
        }

        void push_back(const T& element)
        {
            m_pointers[m_back_index] = m_allocator.allocate(1); 
            m_allocator.construct(m_pointers[m_back_index], element);
            m_back_index++;
        }

        T pop_front()
        {
            if (m_front_index > m_back_index)
            {
                throw std::out_of_range("The container is empty");
            }

            T* front_ptr = m_pointers[m_front_index];
            m_front_index++;
            T front = *front_ptr;

            m_allocator.destroy(front_ptr);
            m_allocator.deallocate(front_ptr, 1);

            return front;
        }

    private:
        allocator m_allocator;
        T* m_pointers[S];
        size_t m_front_index;
        size_t m_back_index;
};