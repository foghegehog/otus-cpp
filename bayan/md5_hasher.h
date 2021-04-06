#ifndef MD5_HASH_H
#define MD5_HASH_H


#include "hasher.h"

class Md5Hasher: public Hasher
{
public:
    std::vector<uint8_t> hash_block(std::vector<uint8_t> block) override;
};

#endif