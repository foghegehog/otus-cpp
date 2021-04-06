#include <boost/uuid/detail/md5.hpp>

#include "md5_hasher.h"

using namespace std;
using namespace boost::uuids::detail;


vector<uint8_t> Md5Hasher::hash_block(vector<uint8_t> block)
{
    md5 hash;
    md5::digest_type digest;

    hash.process_bytes(block.data(), block.size());
    hash.get_digest(digest);
    auto byte_array = reinterpret_cast<uint8_t *>(digest);
    return vector<uint8_t>(byte_array, byte_array + sizeof(digest));
}


