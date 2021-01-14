#include "ip_address.h"

#include <algorithm>
#include <string>
#include <vector>

IpAddress::IpAddress(const string &str)
{
    auto ip_strs = split(str, '.');
    int i = 0;

    for (auto const &part: ip_strs)
    {
        m_bytes[i++] = stoi(part);
    }
}

IpAddress::IpAddress(const initializer_list<int> &bytes)
{
    int i = 0;
    for (auto const &byte: bytes)
    {
        m_bytes[i++] = (uint8_t)byte;
    }
}

bool IpAddress::operator==(const IpAddress &other ) const 
{
     return this->m_bytes == other.m_bytes; 
}

bool IpAddress::operator>=(const IpAddress &other ) const 
{
     return this->m_bytes >= other.m_bytes; 
}

bool IpAddress::operator<=(const IpAddress &other ) const 
{
     return this->m_bytes <= other.m_bytes; 
}

bool IpAddress::operator>(const IpAddress &other ) const 
{
     return this->m_bytes > other.m_bytes; 
}

bool IpAddress::operator<(const IpAddress &other ) const 
{
     return this->m_bytes < other.m_bytes; 
}

bool IpAddress::operator!=(const IpAddress &other ) const 
{
     return this->m_bytes != other.m_bytes; 
}

array<uint8_t, 4> IpAddress::parse_ip(const string &str)
{
    auto ip_strs = split(str, '.');
    array<uint8_t, 4> bytes;

    int i = 0;
    for (auto const &part: ip_strs)
    {
        bytes[i] = stoi(part);
    }

    return bytes;
}

bool IpAddress::ContainsByte(uint8_t searched_byte) const
{
    return (find(m_bytes.begin(), m_bytes.end(), searched_byte) != m_bytes.end()); 
}

void IpAddress::output_ip(ostream& os) const
{
    auto separator = "";
    for_each(this->m_bytes.cbegin(), this->m_bytes.cend(), [&os, &separator](uint8_t part)
    {
        os << separator << (int)part;
        separator = ".";
    });

    os << endl;
}

ostream& operator<<(std::ostream& os, const IpAddress &ip)
{
    ip.output_ip(os);
}