#include "string_lib.h"
#include "ip_address.h"

#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

void find_ip_range(
    const multiset<IpAddress> &ip_index,
    const IpAddress &not_less_ip,
    const IpAddress &not_greater_ip)
{
    auto in_range_iterator = ip_index.upper_bound(not_greater_ip);
    if (in_range_iterator != ip_index.end())
    {
        while(--in_range_iterator != ip_index.end() && *in_range_iterator >= not_less_ip)
        {
            cout << *in_range_iterator;
        }
    }
    // That means that either the last element is less than upper boundary, 
    // or there are no elemets greater than lower boundary
    else
    {
        auto in_range_iterator = ip_index.rbegin();
        while(in_range_iterator != ip_index.rend() && *in_range_iterator >= not_less_ip)
        {
            cout << *in_range_iterator;
            in_range_iterator++;
        }
    }

}

int main()
{
    try
    {
        // vector<vector<string>> ip_pool;

        // As the task requires not only sorting but also searching, some 'indexing' structure will be useful.
        // Multisets are chosen because they are implemented with binary search trees 
        // having O(log(n)) search complexity and ordered iterator over sorted elements.
        //multiset<vector<uint8_t>> ip_pool;
        multiset<IpAddress> ip_pool;

        for(string line; getline(cin, line); )
        {
            auto ip = split(line, '\t').at(0);
            ip_pool.insert(IpAddress(ip));
        }

        // TODO reverse lexicographically sort
        // Vector comparison is performed in lexicographical order.
        // As multiset iterator returns sorted items, 
        // use reverse iterator to obtain reverse lexicographical order.
        for(auto reverse_iterator = ip_pool.rbegin(); reverse_iterator != ip_pool.rend(); reverse_iterator++)
        {
            cout << *reverse_iterator;
        }

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output
        // ip = filter(1)
       find_ip_range(
            ip_pool,
            IpAddress({1, 0, 0, 0}),
            IpAddress({1, 255, 255, 255}));

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)
        find_ip_range(
            ip_pool,
            IpAddress({46, 70, 0, 0}),
            IpAddress({46, 70, 255, 255}));            

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output

        // Cannot invent more effective algorithm than just iterating through.
        // After all, full-record search is usually slow.
        // ip = filter_any(46)
        auto searched_byte = (uint8_t)46;
        for(auto reverse_iterator = ip_pool.rbegin(); reverse_iterator != ip_pool.rend(); reverse_iterator++)
        {
            if(reverse_iterator->ContainsByte(searched_byte)) 
            {
                cout << *reverse_iterator; 
            }
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
