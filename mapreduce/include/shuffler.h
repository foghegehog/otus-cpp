#ifndef SHUFFLE_H
#define SHUFFLE_H

#include "container_trace.h"

#include <iterator>
#include <map>
#include <numeric>
#include <vector>

template<typename K, typename V>
class shuffler
{
public:
    void run(
        std::vector<std::multimap<K, V>>& after_map,
        std::vector<std::vector<std::pair<K, V>>>& for_reduce);
};


template<typename K, typename V>
void shuffler<K, V>::run(
        std::vector<std::multimap<K, V>>& after_map,
        std::vector<std::vector<std::pair<K, V>>>& for_reduce)
{
    auto count_fold  = [](size_t count, const std::multimap<K, V>& container)
    {
        return count + container.size();
    };

    auto mapped_count = std::accumulate(after_map.begin(), after_map.end(), 0, count_fold);
    auto per_reducer_count = mapped_count / for_reduce.size();

    std::vector<container_trace<std::multimap<K, V>>> containers;
    containers.reserve(after_map.size());
    for(auto& container: after_map)
    {
        containers.emplace_back(container);
    }

    auto min_key_container = containers.begin();
    auto shuffled_count = 0;
    for(auto& reducer: for_reduce)
    {
        reducer.reserve(per_reducer_count);

        while(reducer.size() < per_reducer_count)
        {
            auto min_key = min_key_container->m_pos->first;

            for(auto container_it = containers.begin(); container_it != containers.end(); ++container_it)
            {
                if (container_it->m_pos->first < min_key)
                {
                    min_key_container = container_it;
                    min_key = min_key_container->m_pos->first;
                }
            }

            for(auto container_it = min_key_container; container_it != containers.end(); ++container_it)
            {
                if (container_it->m_pos->first != min_key)
                {
                    continue;
                }

                auto from_iterator = std::make_move_iterator(container_it->m_pos);
                auto end_iterator = std::make_move_iterator(container_it->m_end);
                auto to_iterator = std::make_move_iterator(container_it->m_pos);
                auto count = 0;
                while((to_iterator != end_iterator) && (to_iterator->first == min_key))
                {
                     ++to_iterator;
                     ++count;
                }

                reducer.insert(reducer.end(), from_iterator, to_iterator);
                shuffled_count += count;
                std::advance(container_it->m_pos, count);

                if (shuffled_count == mapped_count)
                {
                    return;
                }                
            }


            if (min_key_container->m_pos == min_key_container->m_end)
            {
                ++min_key_container;
                if (min_key_container == containers.end())
                {
                    min_key_container = containers.begin();
                }                
            }
        }

    }
}

#endif