#include <iostream>
#include <list>
#include <tuple>
#include <vector>

using namespace std;

template <typename I>
decltype((declval<I>() >> 1), (declval<I>() & (unsigned short)1), void())
print_ip(const I &number)
{
    unsigned short mask = 0b11111111;
    for (auto byte_index = sizeof(I) - 1; byte_index > 0; byte_index--)
    { 
        cout << ((number >> byte_index * 8) & mask) << '.';
    }

    cout << (number & mask) << endl;
}

template <typename S>
typename enable_if<is_same<string, S>::value, void>::type
print_ip(const S &str)
{
    cout << str << endl;
}

template <typename C>
decltype(begin(declval<C>()), end(declval<C>()), typename enable_if<!is_same<string, C>::value, void>::type())
print_ip(const C &container)
{
    string separator = "";
    for(const auto part: container)
    {
        cout << separator << part;
        separator = ".";
    }

    cout << endl;
}


template<typename Tuple, size_t level, typename Previous, typename Current, typename... Tail>
struct tuple_printer 
{
};

template<typename Tuple, size_t level, typename Current>
struct tuple_printer<Tuple, level, Current, Current>
{
    static void print_part(const Tuple &t)
    {
        cout << get<level>(t);
    }
};

template<typename Tuple, size_t level, typename Current, typename... Tail>
struct tuple_printer<Tuple, level, Current, Current, Tail...>
{
    static void print_part(const Tuple &t)
    {
        cout <<  get<level>(t) << '.';
        tuple_printer<Tuple, level + 1, Current, Tail...>::print_part(t);
    }
};

template<typename Head, typename...Tail>
void print_tuple(const tuple<Head, Tail...> &t)
{
    tuple_printer<tuple<Head, Tail...>, 0, Head, Head, Tail...>::print_part(t);
    cout << endl;
}

template <typename T>
decltype((tuple_size<T>::value), void())
print_ip(const T &tuple)
{
    print_tuple(tuple);
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
    print_ip( make_tuple(123, 456, 789, 0) ); // 123.456.789.0
    //print_ip( make_tuple(123.0, 456, 789, 0) ); // Compilation error
    return 0;
}
