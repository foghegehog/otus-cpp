#include <chrono>
#include <iostream>
#include <future>
#include <thread>

#include "async.h"

int main(int, char *[]) {
    std::size_t bulk = 5;
    auto h = async::connect(bulk);
    auto h2 = async::connect(bulk);
    auto task1 = std::async(std::launch::async, async::receive, h, "1", 1);
    auto task2 = std::async(std::launch::async, async::receive, h2, "1\n", 2);
    task1.wait();
    auto task3 = std::async(std::launch::async, async::receive, h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
    auto task4 = std::async(std::launch::deferred, async::receive, h, "b\nc\nd\n}\n89\n", 11);
    task3.wait();
    task4.wait();
    
    auto fh3 = std::async(std::launch::async, async::connect, bulk);
    auto h3 = fh3.get();
    auto task5 = std::async(std::launch::async, async::receive, h3, "h3_1\nh3_2\n{\nh3_a\nh3_b", 21);
    task5.wait();

    async::disconnect(h);
    async::disconnect(h2);
    async::disconnect(h3);

    return 0;
}
