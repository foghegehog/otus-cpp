#pragma once

#include <array>
#include <tuple>

using namespace std;

template <typename Single, size_t N, typename Head, typename...Tail>
struct tuple_repeat {
    using tuple_type = typename tuple_repeat<Single, N - 1, Head, Head, Tail...>::tuple_type;
};

template <typename Single, typename Repeated, typename...Tail>
struct tuple_repeat<Single, 2, Repeated, Tail...> {
    using tuple_type = std::tuple<Repeated, Repeated, Single>;
};

template <typename Single, typename Repeated>
struct tuple_repeat<Single, 1, Repeated> {
    using tuple_type = std::tuple<Repeated, Single>;
};

// Convert array and single value into a tuple
template<typename Array, typename Single, std::size_t... I>
auto create_tuple_with_indices(const Array& a, std::index_sequence<I...>, Single s)
{
    return std::make_tuple(a[I]..., s);
}
 
template<typename R, std::size_t repeat, typename S, typename Indices = std::make_index_sequence<repeat>>
auto create_tuple_repeat(const std::array<R, repeat>& a, S single)
{
    return create_tuple_with_indices(a, Indices{}, single);
}