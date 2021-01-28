#include "reserving_allocator.h"

#include <cassert>
#include <iostream>
#include <map>


int main(int, char *[]) 
{
    auto standard_map = std::map<int, int, std::less<int>>();
    standard_map[0] = 1;
    for (int i = 1; i <= 9; ++i)
    {
        standard_map[i] = standard_map[i-1] * i;
    }

    auto reserved_map = std::map<
        int,
        int,
        std::less<int>,
        reserving_allocator<std::pair<int, int>, 10>>{};

    reserved_map[0] = 1;
    for (int i = 1; i <= 9; ++i)
    {
        reserved_map[i] = reserved_map[i-1] * i; 
    }

    for (int i = 0; i <= 9; ++i)
    {
        assert(standard_map[i] == reserved_map[i]);
        std::cout << i << " " << reserved_map[i] << std::endl;
    }

    return 0;
}