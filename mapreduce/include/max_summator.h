#ifndef MAX_COUNTER_H
#define MAX_COUNTER_H

#include <map>
#include <vector>

template<typename K, typename V>
class max_summator
{
public:
    void add(const std::pair<K, V>& pair);
    V get_max_value() const;
    std::vector<std::string> get_max_value_str() const;
    std::vector<std::string> get_pairs_str() const;
private:
    std::map<K, V> m_keys_sum;
    typename std::map<K, V>::iterator m_max_pair = m_keys_sum.end();
};

template<typename K, typename V>
void max_summator<K, V>::add(const std::pair<K, V>& pair)
{
    auto existing = m_keys_sum.find(pair.first);
    if (existing == m_keys_sum.end())
    {
        auto result = m_keys_sum.emplace(pair);
        if ((m_max_pair == m_keys_sum.end()) || (m_max_pair->second < pair.second))
        {
            m_max_pair = result.first;
        }
    }
    else
    {
        existing->second += pair.second;
        if (m_max_pair->second < existing->second)
        {
            m_max_pair = existing;
        }
        
    }
}

template<typename K, typename V>
V max_summator<K, V>::get_max_value() const
{
    if (m_max_pair != m_keys_sum.end())
    {
        return m_max_pair->second;
    }
    else
    {
        return -1;
    } 
}

template<typename K, typename V>
std::vector<std::string> max_summator<K, V>::get_max_value_str() const
{
    std::vector<std::string> result = { std::to_string(get_max_value()) };
    return result;
}

template<typename K, typename V>
std::vector<std::string> max_summator<K, V>::get_pairs_str() const
{
    std::vector<std::string> result(m_keys_sum.size());
    for(const auto& pair: m_keys_sum)
    {
        result.emplace_back(pair.first + " " + std::to_string(pair.second));
    }
    return result;
}

#endif