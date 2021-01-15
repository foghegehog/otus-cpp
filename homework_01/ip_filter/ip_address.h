#pragma once

#include "string_lib.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <typename T, ushort L, char S>
class IpAddress
{
    public:
        IpAddress(const string &str);
        IpAddress(const initializer_list<int> &bytes);

        bool operator==(const IpAddress &other) const;
        bool operator>=(const IpAddress &other) const;
        bool operator<=(const IpAddress &other) const;
        bool operator>(const IpAddress &other) const;
        bool operator<(const IpAddress &other) const;
        bool operator!=(const IpAddress &other) const;
        void SetByte(int index, T value);
        bool ContainsByte(T byte) const;
        void output_ip(ostream &os) const;

    private:
        array<T, L> m_bytes;
        array<T, L> parse_ip(const string &str);

};

template <typename T, ushort L, char S>
IpAddress<T, L, S>::IpAddress(const string &str)
{
    auto ip_strs = split(str, S);

    if (ip_strs.size() != L)
    {
        auto what = "Incorrect number of parts in ip string '" + str + "'. Expected: " + to_string(L); 
        throw invalid_argument(what);
    }

    int i = 0;
    for (auto const &part: ip_strs)
    {
        try
        {
            m_bytes[i++] = stoi(part);
        }
        catch(const exception &e)
        {
            auto what = "Invalid input string with IP: '" + str + "'. Details: " + e.what();
            throw invalid_argument(what);
        }
    }
}

template <typename T, ushort L, char S>
IpAddress<T, L, S>::IpAddress(const initializer_list<int> &bytes)
{
    int i = 0;
    for (auto const &byte: bytes)
    {
        m_bytes[i++] = (uint8_t)byte;
    }
}

template <typename T, ushort L, char S>
bool IpAddress<T, L, S>::operator==(const IpAddress &other ) const 
{
     return this->m_bytes == other.m_bytes; 
}

template <typename T, ushort L, char S>
bool IpAddress<T, L, S>::operator>=(const IpAddress &other ) const 
{
     return this->m_bytes >= other.m_bytes; 
}

template <typename T, ushort L, char S>
bool IpAddress<T, L, S>::operator<=(const IpAddress &other ) const 
{
     return this->m_bytes <= other.m_bytes; 
}

template <typename T, ushort L, char S>
bool IpAddress<T, L, S>::operator>(const IpAddress &other ) const 
{
     return this->m_bytes > other.m_bytes; 
}

template <typename T, ushort L, char S>
bool IpAddress<T, L, S>::operator<(const IpAddress &other ) const 
{
     return this->m_bytes < other.m_bytes; 
}

template <typename T, ushort L, char S>
bool IpAddress<T, L, S>::operator!=(const IpAddress &other ) const 
{
     return this->m_bytes != other.m_bytes; 
}

template <typename T, ushort L, char S>
array<T, L> IpAddress<T, L, S>::parse_ip(const string &str)
{
    auto ip_strs = split(str, '.');
    array<T, L> bytes;

    int i = 0;
    for (auto const &part: ip_strs)
    {
        bytes[i] = stoi(part);
    }

    return bytes;
}

template <typename T, ushort L, char S>
void IpAddress<T, L, S>::SetByte(int index, T value)
{
    m_bytes[index] = value;
}

template <typename T, ushort L, char S>
bool IpAddress<T, L, S>::ContainsByte(T searched_byte) const
{
    return (find(m_bytes.begin(), m_bytes.end(), searched_byte) != m_bytes.end()); 
}

template <typename T, ushort L, char S>
void IpAddress<T, L, S>::output_ip(ostream& os) const
{
    auto separator = "";
    for_each(this->m_bytes.cbegin(), this->m_bytes.cend(), [&os, &separator](uint8_t part)
    {
        os << separator << (int)part;
        separator = ".";
    });

    os << endl;
}