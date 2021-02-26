#pragma once

#include <array>
#include <tuple>

using namespace std;

// Convert array and single value into a tuple
template<typename Array, typename Single, std::size_t... I>
auto create_tuple_with_indices(const Array& a, std::index_sequence<I...>, Single s)
{
    return std::make_tuple(a[I]..., s);
}
 
template<typename R, std::size_t repeat, typename S, typename Indices = std::make_index_sequence<repeat>>
auto create_tuple(const std::array<R, repeat>& a, S single)
{
    return create_tuple_with_indices(a, Indices{}, single);
}