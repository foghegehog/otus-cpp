#pragma once

#include "reserving_allocator.h"

#include <cstdlib>
#include <stdexcept> 

template<typename T>
struct queue_element
{
    queue_element(T* data, queue_element<T>* next)
     :m_data(data), m_next(next)
    {}

    T* m_data;
    queue_element<T>* m_next;
};

template<typename T, size_t S, typename allocator>
class custom_queue
{
    public:
        void push_back(const T& data)
        {
            auto data_memory = m_data_allocator.allocate(1);
            m_data_allocator.construct(data_memory, data);

            auto element_memory = m_elements_allocator.allocate(1); 
            m_elements_allocator.construct(element_memory, data_memory, &m_dummy_end);
            if (m_front == &m_dummy_end)
            {
                m_front = element_memory;
            }

            if (m_back != &m_dummy_end)
            {
                m_back->m_next = element_memory;
            }

            m_back = element_memory;
        }

        bool has_elements() const
        {
            return m_front != &m_dummy_end;
        }

        T pop_front()
        {
            if (m_front == &m_dummy_end)
            {
                throw std::out_of_range("The container is empty");
            }

            auto front_ptr = m_front;
            auto data = *(m_front->m_data);
            m_front = m_front->m_next;

            free_element(front_ptr);

            return data;
        }

    ~custom_queue()
    {
        while (m_front != &m_dummy_end)
        {
            auto front_ptr = m_front;
            m_front = m_front->m_next;
            free_element(front_ptr);
        }
    }

    private:

        void free_element(queue_element<T>* element_ptr)
        {
            m_data_allocator.destroy(element_ptr->m_data);
            m_data_allocator.deallocate(element_ptr->m_data, 1);

            m_elements_allocator.destroy(element_ptr);
            m_elements_allocator.deallocate(element_ptr, 1);
        }

       reserving_allocator<queue_element<T>, S> m_elements_allocator;
       allocator m_data_allocator;
       queue_element<T> m_dummy_end{nullptr, nullptr};
       queue_element<T>* m_front = &m_dummy_end;
       queue_element<T>* m_back = &m_dummy_end; 
};