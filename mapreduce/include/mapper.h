#ifndef MAPPER_H
#define MAPPER_H

#include <fstream>
#include <functional>
#include <string>
#include <vector>
#include <utility>

template<typename K, typename V>
class mapper
{
public:
    mapper(
        std::function<std::pair<K, V>> map_func,
        std::string path,
        size_t start,
        size_t block_length)
        : m_map(map_func), m_path(path), m_block_start(start), m_block_length(block_length)
    {}

    std::vector<std::pair<K, V>> run();

private:
    std::function<std::pair<K, V>> m_map;
    std::string m_path;
    size_t m_block_start;
    size_t m_block_length;
};

template<typename K, typename V>
mapper<K, V> run()
{    
    std::ifstream infile(m_path);
    infile.seekg(m_block_start, infile.beg);
    std::vector<std::pair<K, V>> mapped;
    std::string line;
    size_t read_count = 0;
    while((read_count < m_block_length) && std::getline(infile, line))
    {
        read_count += line.size();
        mapped.push_back(m_map(line));
    }

    return mapped;
}

#endif