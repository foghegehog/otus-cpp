#include "custom_queue.h"
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

    /*auto reserved_map = std::map<
        int,
        int,
        std::less<int>,
        reserving_allocator<std::pair<int, int>, 3>>{};*/

    reserved_map[0] = 1;
    for (int i = 1; i <= 9; ++i)
    {
        reserved_map[i] = reserved_map[i-1] * i; 
    }

    for (int i = 0; i <= 9; ++i)
    {
        assert(standard_map[i] == reserved_map[i]);
        std::cout << i << " " << reserved_map[i] << std::endl;
        //reserved_map.erase(i);
        //reserved_map[i] = i;
    }

    auto custom_container = custom_queue<int, 10, std::allocator<int>>();
    for (int i = 0; i <= 9; ++i)
    {
        custom_container.push_back(i);
    }

    auto custom_reserving_container = custom_queue<int, 10, reserving_allocator<int, 10>>();
    //auto custom_reserving_container = custom_queue<int, 10, reserving_allocator<int, 7>>();
    for (int i = 0; i <= 9; ++i)
    {
        custom_reserving_container.push_back(i);
    }

    for (int i = 0; i <= 9; ++i)
    {
        auto custom_front = custom_container.pop_front();
        auto custom_reserving_front = custom_reserving_container.pop_front();

        assert(custom_front == i);
        assert(custom_reserving_front == i);

        std::cout << custom_reserving_front << std::endl;
    }

    return 0;
}