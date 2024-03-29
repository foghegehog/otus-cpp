#ifndef HASHER_H
#define HASHER_H

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

class Hasher
{
public:
    virtual std::vector<uint8_t> hash_block(std::vector<uint8_t> block) = 0;
};

#endif