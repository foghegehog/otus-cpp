#include <iostream>
#include <future>

#include "async.h"

int main(int, char *[]) {
    std::size_t bulk = 5;
    auto h = async::connect(bulk);
    auto h2 = async::connect(bulk);
    auto task1 = std::async(std::launch::deferred | std::launch::async, async::receive, h, "1", 1);
    task1.wait();
    auto task2 = std::async(std::launch::deferred | std::launch::async, async::receive, h2, "1\n", 2);
    task2.wait();
    auto task3 = std::async(std::launch::deferred | std::launch::async, async::receive, h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
    task3.wait();
    auto task4 = std::async(std::launch::deferred | std::launch::async, async::receive, h, "b\nc\nd\n}\n89\n", 11);
    task4.wait();
    /*async::disconnect(h);
    async::disconnect(h2);*/

    return 0;
}
