#ifndef REDUCER_H
#define REDUCER_H

#include <functional>

template<typename Accumulate, typename Key, typename Value>
class reducer
{
public:
    reducer(
        const std::function<void(Accumulate&, const std::pair<Key, Value>&)>& reduce_func)
        : m_reduce(reduce_func)
    {}


    Accumulate run(const std::vector<std::pair<Key, Value>>& pairs);
private:
    std::function<void(Accumulate&, const std::pair<Key, Value>&)> m_reduce;
};

template<typename Accumulate, typename Key, typename Value>
Accumulate reducer<Accumulate, Key, Value>::run(const std::vector<std::pair<Key, Value>>& pairs)
{
    Accumulate accumulate;
    for(const auto& pair: pairs)
    {
        m_reduce(accumulate, pair);
    }

    return accumulate;
}

#endif