#pragma once

#include "ip_address.h"

#include <set>

typedef IpAddress<uint8_t, 4, '.'> IPv4;

class StorageIterator
{
    public:
        StorageIterator(
            const multiset<IPv4>::const_iterator &begin,
            const multiset<IPv4>::const_iterator &end);
        static StorageIterator create_empty();
        bool elements_available() const;
        IPv4 get_current() const;
        void move_next(); 
    private:
        StorageIterator();
        multiset<IPv4>::const_iterator m_current;
        const multiset<IPv4>::const_iterator m_last;
        const multiset<IPv4>::const_iterator m_not_existent;
        bool m_no_more_elements = false;
};

class ReverseStorage
{
    public:
        void insert(const IPv4 &ip);
        StorageIterator get_all();
        StorageIterator find_prefixed(vector<uint8_t> prefix);
        vector<IPv4> find_containing_byte(uint8_t searched_byte);
    private:
    // As the task requires not only sorting but also searching, some 'indexing' structure will be useful.
    // Multisets are chosen because they are implemented with binary search trees 
    // having O(log(n)) search complexity and ordered iterator over sorted elements.
    multiset<IPv4> m_storage;
};