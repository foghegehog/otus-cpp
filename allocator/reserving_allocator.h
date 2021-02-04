#pragma once

#include "memory_block.h"

#include <cstdlib>
#include <forward_list>
#include <new>

template<typename T, size_t M>
struct reserving_allocator 
{
    using value_type = T;

    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    template<typename U>
    struct rebind 
    {
        using other = reserving_allocator<U, M>;
    };

    const size_t MAX_ELEMENTS = M;

    std::forward_list<memory_block<T, M>> m_blocks;
    void* m_memory = nullptr;
    size_t m_allocated_count = 0;

    reserving_allocator() = default;
    ~reserving_allocator()
    {
        if (m_memory != nullptr)
        {
            free(m_memory);
            m_memory = nullptr;
            m_allocated_count = 0;
        }
    }

    template<typename U, size_t N> 
    reserving_allocator(const reserving_allocator<U, N>&) 
    {
    }

    void allocate_memory_block()
    {
        m_memory = std::malloc(MAX_ELEMENTS * sizeof(T));
        if (!m_memory)
            throw std::bad_alloc(); 
                
        auto block = memory_block<T, M>{reinterpret_cast<T *>(m_memory)};
        m_blocks.emplace_front(block);
    }

    T *allocate(std::size_t n) 
    {
        if (n > MAX_ELEMENTS)
        {
            throw std::bad_alloc(); 
        }

        if (m_blocks.empty())
        {
            allocate_memory_block();
        }

        for(auto& block : m_blocks)
        {
            auto free_area = block.try_get_free_memory(n);
            if (free_area != nullptr)
            {
                return free_area;
            }
        }

        allocate_memory_block();
        auto free_area = m_blocks.front().try_get_free_memory(n);
        if (free_area != nullptr)
        {
            return free_area;
        }
        else
        {
            throw std::bad_alloc();
        }
        

    }

    void deallocate(T *p, std::size_t n) 
    {
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) 
    {
        new(p) U(std::forward<Args>(args)...);
    };

    void destroy(T *p) 
    {
        p->~T();
    }
};