#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <tuple>

using namespace std;

template<typename...Tail>
class custom_tuple
{
    public:
        custom_tuple(){};
        custom_tuple(const custom_tuple &other){};
        custom_tuple& operator=(const custom_tuple &other)
        {
            return *this; 
        }
};

template<typename Head, typename...Tail>
class custom_tuple<Head, Tail...> : public custom_tuple<Tail...>
{
    public:
        custom_tuple(){};
         custom_tuple(const Head &head, const Tail&...tail)
            :member(head), custom_tuple<Tail...>(tail...)
        {
        };

        template<typename OHead, typename...OTail>
        custom_tuple<Head, Tail...>& operator=(const custom_tuple<OHead, OTail...> &other)
        {
            member = other.member;
            auto ocast = dynamic_cast<const custom_tuple<OTail...>&>(other);
            dynamic_cast<custom_tuple<Tail...>*>(this)->operator=(ocast);
            return *this; 
        }

        Head member;
};

template<size_t index, typename Head, typename...Tail>
struct Getter
{
    public:
        static auto get(const custom_tuple<Head, Tail...> &t) -> decltype(Getter<index-1, Tail...>::get(t))
        {
            return Getter<index-1, Tail...>::get(t);
        };
};

template<typename Head, typename...Tail>
struct Getter<0, Head, Tail...>
{
    public:
        static Head get(const custom_tuple<Head, Tail...> &t)
        {
            return t.member;
        }
};

template<int index, typename...Args>
auto get(const custom_tuple<Args...>& t) -> decltype(Getter<index, Args...>::get(t))
{
    return Getter<index, Args...>::get(t);
}


template <typename... Args>
auto custom_tie(Args&... args) {
    return custom_tuple<Args&...>(args...);
}

auto getPerson() 
{
    const string name = "Petia";
    const string secondName = "Ivanoff";
    const size_t age = 23;
    const string department = "Sale";
    return custom_tuple<string, string, size_t, string>(name, secondName, age, department);
}

int main(int argc, char * argv[]) 
{
    string name, secondName, department;
    size_t age;
    auto person = getPerson();
    assert(get<0>(person) == "Petia");
    assert(get<1>(person) == "Ivanoff");
    assert(get<2>(person) == 23);
    assert(get<3>(person) == "Sale");
    custom_tie(name, secondName, age, department) = getPerson();
    assert(name == "Petia");
    assert(secondName == "Ivanoff");
    assert(age == 23);
    assert(department == "Sale");
    cout << name << endl;
    cout << secondName << endl;
    cout << age << endl;
    cout << department << endl;
    return 0;
}