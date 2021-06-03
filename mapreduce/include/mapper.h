#ifndef MAPPER_H
#define MAPPER_H

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
        const std::string& path,
        size_t start,
        size_t block_length)
        : m_map(map_func), m_path(path), m_block_start(start), m_block_length(block_length)
    {}

    void run(std::multimap<K, V>& container);

private:
    std::function<std::pair<K, V>(std::string)> m_map;
    std::string m_path;
    size_t m_block_start;
    size_t m_block_length;
};

template<typename K, typename V>
void run(std::multimap<K, V>& container)
{    
    std::ifstream infile(m_path);
    infile.seekg(m_block_start, infile.beg);
    std::string line;
    size_t read_count = 0;
    while((read_count < m_block_length) && std::getline(infile, line))
    {
        read_count += line.size();
        container.insert(m_map(line));
    }
}

#endif