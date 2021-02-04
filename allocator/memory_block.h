#pragma once

#include <cstdlib>
#include <forward_list>

template<typename T>
class free_area
{
    public:
        free_area(T* start, size_t count)
        :m_start(start), m_count(count)
        {}

        bool accommodates(size_t n) const
        {
            return m_count >= n;
        }

        T* occupy(size_t n)
        {
            auto occupied = m_start;
            m_start += n;
            m_count -= n;

            return occupied;
        }

        bool is_empty() const
        {
            return m_count == 0;
        }

    private:
        T* m_start;
        size_t m_count;
};

template<typename T, size_t M>
class memory_block
{
    public:
        memory_block(T* memory_start)
            :m_memory{memory_start}
        {
            m_free_areas.emplace_front(free_area<T>{memory_start, M});
        }

        T* try_get_free_memory(size_t n)
        {
            auto prev = m_free_areas.before_begin();
            for(auto& current : m_free_areas)
            {
                if (current.accommodates(n))
                {
                    auto occupied = current.occupy(n);
                    if (current.is_empty())
                    {
                        m_free_areas.erase_after(prev);
                    }

                    return occupied;
                }

                ++prev;     
            }

            return nullptr;

        }

        T* get_memory()
        {
            return m_memory;
        }

    private:
        T* m_memory;
        const size_t MAX_ELEMENTS = M;
        std::forward_list<free_area<T>> m_free_areas;
};