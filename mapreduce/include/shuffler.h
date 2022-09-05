#ifndef SHUFFLE_H
#define SHUFFLE_H

#include "container_trace.h"

#include <iterator>
#include <list>
#include <numeric>
#include <vector>

template<typename K, typename V>
class shuffler
{
public:
    void run(
        std::vector<std::vector<std::pair<K, V>>>& after_map,
        std::vector<std::vector<std::pair<K, V>>>& for_reduce);
};


template<typename K, typename V>
void shuffler<K, V>::run(
        std::vector<std::vector<std::pair<K, V>>>& after_map,
        std::vector<std::vector<std::pair<K, V>>>& for_reduce)
{
    auto count_fold  = [](size_t count, const std::vector<std::pair<K, V>>& container)
    {
        return count + container.size();
    };

    auto mapped_count = std::accumulate(after_map.begin(), after_map.end(), 0, count_fold);
    auto per_reducer_count = mapped_count / for_reduce.size();
    if (mapped_count % for_reduce.size() != 0)
    {
        per_reducer_count += 1;
    }

    // Traces current non-copied position of each non-traversed container
    std::list<container_trace<std::vector<std::pair<K, V>>>> containers;
    for(auto& container: after_map)
    {
        if (!container.empty())
        {
            containers.emplace_back(container);
        }
    }

    auto reducer = for_reduce.begin();
    reducer->reserve(per_reducer_count);

    // Extrapolate merge step of the merge sort algorithm to the case of multiple containers instead of 2
    while(!containers.empty())
    {
        // Find the first container with current copying position having key less then of all others containers' pos
        auto min_key_container = containers.begin();
        auto min_key = min_key_container->m_pos->first;

        for(auto container_it = next(min_key_container); container_it != containers.end(); ++container_it)
        {
            if (container_it->m_pos->first < min_key)
            {
                min_key_container = container_it;
                min_key = min_key_container->m_pos->first;
            }
        }

        // For all containers with position containing min key copy min key elements to target
        for(auto container_it = min_key_container; container_it != containers.end(); ++container_it)
        {
            // Skip if not the case
            if (container_it->m_pos->first != min_key)
            {
                continue;
            }

            // Get iterators for range of positions with min key
            auto from_iterator = std::make_move_iterator(container_it->m_pos);
            auto end_iterator = std::make_move_iterator(container_it->m_end);
            auto to_iterator = std::make_move_iterator(container_it->m_pos);
            auto count = 0;
            while((to_iterator != end_iterator) && (to_iterator->first == min_key))
            {
                ++to_iterator;
                ++count;
            }

            // Copy all min key elements of the current container to target
            reducer->insert(reducer->end(), from_iterator, to_iterator);

            // Advance current copying position for amount of copyed elements
            std::advance(container_it->m_pos, count);

            // If all elements of the container were copied, remove the container from the list
            if (container_it->is_finished())
            {
                auto to_remove = container_it;
                // Will be increased in the loop iteration to the element next to removed 
                --container_it; 
                containers.erase(to_remove);
            }
        }

        // If target reducer is full, move to next one
        if (reducer->size() >= per_reducer_count)
        {
            ++reducer;
            reducer->reserve(per_reducer_count);
        }
    }
}

#endif