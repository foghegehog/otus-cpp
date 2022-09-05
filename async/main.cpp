#include <chrono>
#include <iostream>
#include <future>
#include <thread>

#include "async.h"

int main(int, char *[]) {
    std::size_t bulk = 5;
    auto h = async::connect(bulk);
    auto h2 = async::connect(bulk);
    auto task_h_command1 = std::async(std::launch::async, async::receive, h, "1", 1);
    auto task_h2_command1 = std::async(std::launch::async, async::receive, h2, "1\n", 2);
    task_h_command1.wait();
    auto task_h_bulk_unfinished_bulk_dynamic = std::async(std::launch::async, async::receive, h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
    auto task_h_dynamic_unfinished_bulk = std::async(std::launch::deferred, async::receive, h, "b\nc\nd\n}\n89\n", 11);
    task_h_bulk_unfinished_bulk_dynamic.wait();
    task_h_dynamic_unfinished_bulk.wait();
    async::disconnect(h);
    
    auto fh3 = std::async(std::launch::async, async::connect, bulk);
    auto h3 = fh3.get();
    auto task_h3_commands = std::async(std::launch::async, async::receive, h3, "h3_1\nh3_2\n{\nh3_a\nh3_b", 21);

    task_h2_command1.wait();
    async::disconnect(h2);

    task_h3_commands.wait();
    async::disconnect(h3);

    async::Context::stop_background_workers();
    //std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}
