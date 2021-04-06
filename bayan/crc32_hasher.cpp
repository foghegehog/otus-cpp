#include "crc32_hasher.h"

#include <boost/crc.hpp> 
#include <vector>

using namespace boost;
using namespace std;

vector<uint8_t> Crc32Hasher::hash_block(vector<uint8_t> block)
{
    crc_32_type crc32;
    crc32.process_bytes(block.data(), block.size());
    unsigned int checksum = crc32.checksum();
    auto byte_array = reinterpret_cast<uint8_t *>(&checksum);
    return vector<uint8_t>(byte_array, byte_array + sizeof(checksum));
}