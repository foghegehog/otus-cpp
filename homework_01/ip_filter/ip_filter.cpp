#include <algorithm>
#include <array>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <set>
#include <tuple>
#include <string>
#include <vector>

using namespace std;
typedef tuple<uint8_t, uint8_t, uint8_t, uint8_t> ip_tuple;

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
auto split(const string &str, char d)
{
    vector<string> r;
    string::size_type start = 0;
    for(auto stop = str.find_first_of(d); stop != string::npos; stop = str.find_first_of(d, start))
    {
        r.push_back(str.substr(start, stop - start));
        start = stop + 1;
    }

    r.push_back(str.substr(start));
    return r;
}

auto convert_to_byte_tuple(array<int, 4> ip)
{
    return make_tuple((uint8_t)ip[0], (uint8_t)ip[1], (uint8_t)ip[2], (uint8_t)ip[3]);
}

auto parse_ip(const string &str)
{
    auto ip_strs = split(str, '.');

    return convert_to_byte_tuple(array<int, 4>{
        stoi(ip_strs[0]), stoi(ip_strs[1]), stoi(ip_strs[2]), stoi(ip_strs[3])});
}

void output_ip(ip_tuple ip)
{
    cout << (int)get<0>(ip) << "." << (int)get<1>(ip) << ".";
    cout << (int)get<2>(ip) << "." << (int)get<3>(ip) << endl;
}

void find_ip_range(multiset<ip_tuple> ip_index, ip_tuple not_less_ip, ip_tuple not_greater_ip)
{
    auto upper = ip_index.upper_bound(not_greater_ip);
    if (upper != ip_index.end())
    {
        while(--upper != ip_index.end() && *upper >= not_less_ip)
        {
            output_ip(*upper);
        }
    }
    // That means that either the last element is less than upper boundary, 
    // or there are no elemets greater than lower boundary
    else
    {
        auto upper_reverse = ip_index.rbegin();
        while(upper_reverse != ip_index.rend() && *upper_reverse >= not_less_ip)
        {
            output_ip(*upper_reverse);
            upper_reverse++;
        }
    }

}

int main()
{
    try
    {
        // vector<vector<string>> ip_pool;

        // As the task requires not only sorting but also searching some 'indexing' structure will be useful.
        // Multisets are chosen because they are implemented with binary search trees 
        // having O(log(n)) search complexity and ordered iterator over sorted elements.
        multiset<ip_tuple> ip_pool;

        for(string line; getline(cin, line); )
        {
            auto ip = split(line, '\t').at(0);
            ip_pool.insert(parse_ip(ip));
        }

        // TODO reverse lexicographically sort
        // Tuple comparison is performed in lexicographical order.
        // As multiset iterator returns sorted items, 
        // use reverse iterator to obtain reverse lexicographical order.

        //for(const auto &ip: ip_pool)
        for(auto reverse_iterator = ip_pool.rbegin(); reverse_iterator != ip_pool.rend(); reverse_iterator++)
        {
            output_ip(*reverse_iterator);
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
            convert_to_byte_tuple(array<int, 4> {1, 0, 0, 0}),
            convert_to_byte_tuple(array<int, 4> {1, 255, 255, 255}));
        
        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)
        find_ip_range(
            ip_pool,
            convert_to_byte_tuple(array<int, 4> {46, 70, 0, 0}),
            convert_to_byte_tuple(array<int, 4> {46, 70, 255, 255}));

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // Cannot invent more effective algorithm than just iterating throug.
        // After all, full-record search is usially slow.
        // ip = filter_any(46)
        auto searched_byte = (uint8_t)46;
        for(auto reverse_iterator = ip_pool.rbegin(); reverse_iterator != ip_pool.rend(); reverse_iterator++)
        {
            auto ip = *reverse_iterator;
            if (get<0>(ip) == searched_byte || get<1>(ip) == searched_byte 
                || get<2>(ip) == searched_byte || get<3>(ip) == searched_byte )
            {
                output_ip(*reverse_iterator);
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
