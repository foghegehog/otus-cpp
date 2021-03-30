#ifndef HASHER_H
#define HASHER_H

#include <cstddef>
#include <cstdint>

class Hasher
{
    virtual void hash_block(const uint8_t * block, uint8_t * digest) = 0;
};

#endif