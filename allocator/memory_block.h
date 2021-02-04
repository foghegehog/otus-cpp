#pragma once

#include <cstdlib>
#include <forward_list>
#include <stdexcept> 

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

        bool is_after(const T* address) const
        {
            return address < m_start;
        }

        bool adjoins_left(const T* address) const
        {
            return address == m_start;
        }

        void merge_left(T* area, size_t n)
        {
            m_start = area;
            m_count += n;
        }

        bool is_before(const T* address)
        {
            return m_start + m_count < address;
        }

        bool adjoins_right(const T* address) const
        {
            return m_start + m_count == address;
        }

        void merge_right(T* area, size_t n)
        {
            m_count += n;
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

        bool contains_pointer(const T* area) const
        {
            return (area >= m_memory) && (area < m_memory + MAX_ELEMENTS);
        }

        void mark_as_free(T* area, size_t n)
        {
            if ((area < m_memory) || (area >= m_memory + MAX_ELEMENTS))
                throw std::out_of_range("Attempt to free pointer in the wrong block");

            auto area_end = area + n;
            auto prev_free = m_free_areas.before_begin();
            for (auto current_free = m_free_areas.begin(); current_free != m_free_areas.end(); ++current_free)
            {
                if (current_free->is_after(area_end))
                {
                    auto new_free_area = free_area<T>{area, n};
                    m_free_areas.emplace_after(prev_free, new_free_area);
                    return;
                }

                if (current_free->adjoins_left(area_end))
                {
                    current_free->merge_left(area_end, n);
                    return;
                }
                
                if (current_free->adjoins_right(area))
                {
                    current_free->merge_right(area, n);
                    return;
                }

                prev_free = current_free;
            }

            // iterated through the whole list, new area is after the last element
            auto new_free_area = free_area<T>{area, n};
            m_free_areas.emplace_after(prev_free, new_free_area);
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