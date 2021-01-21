#include "string_lib.h"
#include "ip_address.h"
#include "ip_storage.h"

#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

ostream& operator<<(std::ostream& os, const IPv4 &ip)
{
    ip.output_ip(os);
    return os;
}

void output_ips(StorageIterator &storage_iterator)
{
    while (storage_iterator.elements_available())
    {
        cout << storage_iterator.get_current();
        storage_iterator.move_next();
    }
}

int main()
{
    try
    {
        ReverseStorage ip_storage;

        for(string line; getline(cin, line); )
        {
            auto ip_str = split(line, '\t').at(0);
            ip_storage.insert(IPv4(ip_str));
        }

        // TODO reverse lexicographically sort
        auto reverse_iterator = ip_storage.get_all();
        output_ips(reverse_iterator);       

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output
        // ip = filter(1)
       auto filter_1_iterator = ip_storage.find_prefixed(vector<uint8_t>({1}));
       output_ips(filter_1_iterator);

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)
        auto filter_46_70_iterator = ip_storage.find_prefixed(vector<uint8_t>({46, 70}));
        output_ips(filter_46_70_iterator);            

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
         // ip = filter_any(46)
        vector<IPv4> filter_any_46 = ip_storage.find_containing_byte((uint8_t)46);
        for(const auto &ip: filter_any_46)
        {
            cout << ip;
        }

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch(const exception &e)
    {
        cerr << e.what() << endl;
    }

    return 0;
}
