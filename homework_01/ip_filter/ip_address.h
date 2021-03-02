#pragma once

#include "string_lib.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;


/*!
 * Generalized IP address.
 * @tparam  T  The type to store parts of IP.
 * @tparam  L  The amout of parts in the IP address. 
 * @tparam  S  The separator used in parsing and output of IP address.
 */
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
        template<typename SourceEnumerable, typename Part>
        void InitializeFrom(const SourceEnumerable& source, function<T(Part)> parse);

};

template <typename T, ushort L, char S>
IpAddress<T, L, S>::IpAddress(const string &str)
{
    auto ip_strs = split(str, S);

    InitializeFrom<vector<string>, string>(ip_strs, [](string str){ return stoi(str);});
}

template <typename T, ushort L, char S>
IpAddress<T, L, S>::IpAddress(const initializer_list<int> &bytes)
{
    InitializeFrom<initializer_list<int>, int>(bytes, [](int byte){ return (T)byte;});
}

template <typename T, ushort L, char S>
bool IpAddress<T, L, S>::operator==(const IpAddress &other ) const 
{
     return m_bytes == other.m_bytes; 
}

template <typename T, ushort L, char S>
bool IpAddress<T, L, S>::operator>=(const IpAddress &other ) const 
{
     return m_bytes >= other.m_bytes; 
}

template <typename T, ushort L, char S>
bool IpAddress<T, L, S>::operator<=(const IpAddress &other ) const 
{
     return m_bytes <= other.m_bytes; 
}

template <typename T, ushort L, char S>
bool IpAddress<T, L, S>::operator>(const IpAddress &other ) const 
{
     return m_bytes > other.m_bytes; 
}

template <typename T, ushort L, char S>
bool IpAddress<T, L, S>::operator<(const IpAddress &other ) const 
{
     return m_bytes < other.m_bytes; 
}

template <typename T, ushort L, char S>
bool IpAddress<T, L, S>::operator!=(const IpAddress &other ) const 
{
     return m_bytes != other.m_bytes; 
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
    auto separator = '\0';
    for_each(m_bytes.cbegin(), m_bytes.cend(), [&os, &separator](uint8_t part)
    {
        os << separator << (int)part;
        separator = S;
    });

    os << endl;
}

template <typename T, ushort L, char S>
template<typename SourceEnumerable, typename Part>
void IpAddress<T, L, S>::InitializeFrom(const SourceEnumerable& source, function<T(Part)> parse)
{
    if (source.size() != L)
    {
        auto what = "Incorrect number of parts in ip string. Expected: " + to_string(L); 
        throw invalid_argument(what);
    }

    int i = 0;
    for (auto const &part: source)
    {
        try
        {
            m_bytes[i++] = parse(part);
        }
        catch(const exception &e)
        {
            auto what = string("Invalid IP address. Details: ") + e.what();
            throw invalid_argument(what);
        }
    }
}