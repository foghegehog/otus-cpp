#ifndef CRC32_HASHER_H
#define CRC32_HASHER_H

#include "hasher.h"

class Crc32Hasher: public Hasher
{
public:
    std::vector<uint8_t> hash_block(std::vector<uint8_t> block) override;
};

#endif