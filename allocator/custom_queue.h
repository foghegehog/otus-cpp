#pragma once

#include "reserving_allocator.h"

#include <cstddef>
#include <cstdlib>
#include <iterator>
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
        struct Iterator 
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = T;
            using pointer           = T*;
            using reference         = T&;

            Iterator(queue_element<T>* ptr) : m_ptr(ptr) {}

            reference operator*() const { return *(m_ptr->m_data); }
            pointer operator->() { return m_ptr->m_data; }
            Iterator& operator++() 
            {
                m_ptr = m_ptr->m_next; 
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
                return a.m_ptr == b.m_ptr; 
            };

            friend bool operator!= (const Iterator& a, const Iterator& b)
            {
                return a.m_ptr != b.m_ptr; 
            };  

        private:
            queue_element<T>* m_ptr;
        };

        Iterator begin() { return Iterator(m_front); }
        Iterator end()   { return Iterator(&m_dummy_end); }

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