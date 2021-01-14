#pragma once

#include "string_lib.h"

#include <array>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

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
        bool ContainsByte(uint8_t byte) const;
        void output_ip(ostream &os) const;

    private:
        array<uint8_t, 4> m_bytes;
        array<uint8_t, 4> parse_ip(const string &str);

};

ostream& operator<<(std::ostream& os, const IpAddress &ip);