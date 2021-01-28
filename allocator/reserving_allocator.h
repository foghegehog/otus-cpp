#pragma once

#include <cstdlib>
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

    T *allocate(std::size_t n) 
    {
        if (n > MAX_ELEMENTS)
        {
            throw std::bad_alloc(); 
        }

        if (m_memory == nullptr)
        {
            m_memory = std::malloc(MAX_ELEMENTS * sizeof(T));
            if (!m_memory)
                throw std::bad_alloc(); 
        }

        if (m_allocated_count + n <= MAX_ELEMENTS)
        {
            auto allocation = reinterpret_cast<T *>(m_memory) + m_allocated_count; 
            m_allocated_count += n;
            return allocation;
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