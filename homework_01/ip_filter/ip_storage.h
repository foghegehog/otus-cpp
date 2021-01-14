#pragma once

#include "ip_address.h"

#include <set>

typedef IpAddress<uint8_t, 4, '.'> IPv4;

struct View
{
    public:
        View(
            const multiset<IPv4>::const_iterator &start,
            const multiset<IPv4>::const_iterator &stop,
            const multiset<IPv4>::const_iterator &storage_end);
        bool is_empty() const;
        const multiset<IPv4>::const_iterator start;
        const multiset<IPv4>::const_iterator stop;
    private:
        const multiset<IPv4>::const_iterator storage_end;
};

class ReverseStorage
{
    public:
        void insert(const IPv4 &ip);
        View get_all();
        View find_prefixed(vector<uint8_t> prefix);
        vector<IPv4> find_containing_byte(uint8_t searched_byte);
    private:
    // As the task requires not only sorting but also searching, some 'indexing' structure will be useful.
    // Multisets are chosen because they are implemented with binary search trees 
    // having O(log(n)) search complexity and ordered iterator over sorted elements.
        multiset<IPv4> m_storage;
};