#ifndef MAPPER_H
#define MAPPER_H

#include "block_reader.h"

#include <fstream>
#include <functional>
#include <map>
#include <string>
#include <utility>

template<typename K, typename V>
class mapper
{
public:
    mapper(
        const std::function<std::pair<K, V>(const std::string&)>& map_func,
        block_reader * reader)
        : m_map(map_func), m_block_reader(reader)
    {}

    void run(std::multimap<K, V>& container);

private:
    std::function<std::pair<K, V>(const std::string&)> m_map;
    block_reader * m_block_reader;
};

template<typename K, typename V>
void mapper<K, V>::run(std::multimap<K, V>& container)
{    
    std::string line;
    while(m_block_reader->get_next_line(line))
    {
        container.insert(m_map(line));
    }
}

#endif