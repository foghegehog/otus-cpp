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
        const std::function<std::pair<K, V>(std::string)>& map_func,
        block_reader * reader)
        : m_map(map_func), m_block_reader(reader)
    {}

    void run(std::multimap<K, V>& container);

private:
    std::function<std::pair<K, V>(std::string)> m_map;
    block_reader * m_block_reader;
};

template<typename K, typename V>
void run(std::multimap<K, V>& container)
{    
    std::ifstream infile(m_path);
    infile.seekg(m_block_start, infile.beg);
    std::string line;
    while(reader->get_next_line(line))
    {
        read_count += line.size();
        container.insert(m_map(line));
    }
}

#endif