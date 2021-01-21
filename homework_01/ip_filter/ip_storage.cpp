#include "ip_address.h"
#include "ip_storage.h"

#include <iostream>

using namespace std;

void ReverseStorage::insert(const IPv4 &ip)
{
    m_storage.insert(ip);
}

StorageIterator ReverseStorage::get_all()
{
    if (!m_storage.empty())
    {
        // As multiset iterator returns sorted items, 
        // navigate iterator back to get items in reverse lexicographical order.
        auto first = m_storage.cend();
        first--;
        return StorageIterator(first, m_storage.cbegin());
    }
    else
    {
        return StorageIterator::create_empty();
    }
}

StorageIterator ReverseStorage::find_prefixed(vector<uint8_t> prefix)
{
    auto not_less_ip = IPv4({0, 0, 0, 0});
    auto not_greater_ip = IPv4({255, 255, 255, 255});
    for (size_t i = 0; i < prefix.size(); i++)
    {
        not_less_ip.SetByte(i, prefix[i]);
        not_greater_ip.SetByte(i, prefix[i]);
    }
    
    auto first = m_storage.upper_bound(not_greater_ip);
    first--;
    if (*first >= not_less_ip)
    {
        auto last = m_storage.lower_bound(not_less_ip);
        return StorageIterator(first, last);
    }
    else
    {
        return StorageIterator::create_empty();
    }
    
}

vector<IPv4> ReverseStorage::find_containing_byte(uint8_t searched_byte)
{
    vector<IPv4> filtered;
    if (m_storage.empty())
    {
        return filtered;
    }

    // Cannot invent more effective algorithm than just iterating through.
    // After all, full-record search is usually slow.
    auto back_order_iterator = m_storage.end();
    do
    {
        back_order_iterator--;
        if(back_order_iterator->ContainsByte(searched_byte)) 
        {
            filtered.push_back(*back_order_iterator); 
        }
    } 
    while (back_order_iterator != m_storage.begin());
}

StorageIterator::StorageIterator(const multiset<IPv4>::const_iterator &begin,
          const multiset<IPv4>::const_iterator &end)
          :m_current(begin), m_last(end)
{
}

StorageIterator::StorageIterator()
{
    m_no_more_elements = true;
}

StorageIterator StorageIterator::create_empty()
{
    return StorageIterator();
}

bool StorageIterator::elements_available() const
{
    return !m_no_more_elements;
}

IPv4 StorageIterator::get_current() const
{
    return *m_current;
}

void StorageIterator::move_next()
{
    if (m_current == m_last)
    {
        m_no_more_elements = true;
    } 
    else
    {
        m_current--;
    }
}