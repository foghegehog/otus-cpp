#include <iostream>
#include <list>
#include <vector>

using namespace std;

template <typename I>
decltype((declval<I>() >> 1), (declval<I>() & (unsigned short)1), void())
print_ip(I number)
{
    unsigned short mask = 0b11111111;
    for (auto byte_index = sizeof(I) - 1; byte_index > 0; byte_index--)
    { 
        cout << ((number >> byte_index * 8) & mask) << '.';
    }

    cout << (number & mask) << endl;
}

template <typename CharT>
void print_ip(basic_string<CharT> str)
{
    cout << str << endl;
}

template <typename C>
decltype(begin(declval<C>()), end(declval<C>()), void())
print_ip(C container)
{
    string separator = "";
    for(const auto part: container)
    {
        cout << separator << part;
        separator = ".";
    }

    cout << endl;
}

int main(void) 
{
    print_ip( char{-1} ); // 255
    print_ip( short{0} ); // 0.0
    print_ip( int{2130706433} ); // 127.0.0.1
    print_ip( long{8875824491850138409} ); // 123.45.67.89.101.112.131.41
    print_ip( string{"Hello, World!"} ); // Hello, World!
    print_ip( vector<int>{100, 200, 300, 400} ); // 100.200.300.400
    print_ip( list<short>{400, 300, 200, 100} ); // 400.300.200.100
    return 0;
}
