#include "string_lib.h"
#include "ip_address.h"
#include "ip_storage.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>

// ("",  '.') -> [""]
TEST(CheckSplit, EmptyString)
{
    auto result = split("", '.');
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0], "");
}

// ("11", '.') -> ["11"]
TEST(CheckSplit, SingleString)
{
    auto result = split("11", '.');
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0], "11");
}

// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
TEST(CheckSplit, TwoEmptyParts)
{
    auto result = split("..", '.');
    ASSERT_EQ(result.size(), 3);
    for (auto const &part: result)
    {
        ASSERT_EQ(part, "");
    }
}

// ("11.", '.') -> ["11", ""]
TEST(CheckSplit, NonEmptyEmptyParts)
{
    auto result = split("11.", '.');
    ASSERT_EQ(result.size(), 2);
    ASSERT_EQ(result[0], "11");
    ASSERT_EQ(result[1], "");
}

// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
TEST(CheckSplit, EmptyNonEmptyParts)
{
    auto result = split(".11", '.');
    ASSERT_EQ(result.size(), 2);
    ASSERT_EQ(result[0], "");
    ASSERT_EQ(result[1], "11");
}

// ("11.22", '.') -> ["11", "22"]
TEST(CheckSplit, TwoNonEmptyParts)
{
    auto result = split("11.22", '.');
    ASSERT_EQ(result.size(), 2);
    ASSERT_EQ(result[0], "11");
    ASSERT_EQ(result[1], "22");
}

// 222.173.235.246
// 222.130.177.64
// 222.82.198.61
// ...
// 1.70.44.170
// 1.29.168.152
// 1.1.234.8
TEST(CheckFiltration, ReverseLexicographical)
{
    auto expected = 
    {
        IPv4({222, 173, 235, 246}),
        IPv4({222, 130, 177, 64}),
        IPv4({222, 82, 198, 61}),
        IPv4({1, 70, 44, 170}),
        IPv4({1, 29, 168, 152}),
        IPv4({1, 1, 234, 8}),
    };

    ReverseStorage ip_storage;
    ip_storage.insert(IPv4("1.29.168.152"));
    ip_storage.insert(IPv4("1.1.234.8"));
    ip_storage.insert(IPv4("222.82.198.61"));
    ip_storage.insert(IPv4("1.70.44.170"));
    ip_storage.insert(IPv4("222.130.177.64"));
    ip_storage.insert(IPv4("222.173.235.246"));

    auto storage_iterator = ip_storage.get_all();

    int i = 0;
    while (storage_iterator.elements_available())
    {
        auto expected_ip = expected.begin()[i++];
        auto actual_ip = storage_iterator.get_current();
        ASSERT_EQ(expected_ip, actual_ip);

        storage_iterator.move_next();
    }

    ASSERT_EQ(i, expected.size());
}

auto get_ips_with_46()
{
    std::vector<std::string> ips =  
    {
        "186.204.34.46",
        "186.46.222.194",
        "185.46.87.231",
        "185.46.86.132",
        "185.46.86.131",
        "185.46.86.131",
        "185.46.86.22",
        "185.46.85.204",
        "185.46.85.78",
        "68.46.218.208",
        "46.251.197.23",
        "46.223.254.56",
        "46.223.254.56",
        "46.182.19.219",
        "46.161.63.66",
        "46.161.61.51",
        "46.161.60.92",
        "46.161.60.35",
        "46.161.58.202",
        "46.161.56.241",
        "46.161.56.203",
        "46.161.56.174",
        "46.161.56.106",
        "46.161.56.106",
        "46.101.163.119",
        "46.101.127.145",
        "46.70.225.39",
        "46.70.147.26",
        "46.70.113.73",
        "46.70.29.76",
        "46.55.46.98",
        "46.49.43.85",
        "39.46.86.85",
        "5.189.203.46"
    };

    return ips;
}

// 46.70.225.39
// 46.70.147.26
// 46.70.113.73
// 46.70.29.76
TEST(CheckFiltration, FilterPrefixed)
{
    auto expected = 
    {
        IPv4({46, 70, 225, 39}),
        IPv4({46, 70, 147, 26}),
        IPv4({46, 70, 113, 73}),
        IPv4({46, 70, 29, 76})
    };

    ReverseStorage ip_storage;
    auto with_46 = get_ips_with_46();
    for(const auto &ip_str: with_46)
    {
        ip_storage.insert(IPv4(ip_str));
    }

    auto storage_iterator = ip_storage.find_prefixed(vector<uint8_t>({46, 70}));

    int i = 0;
    while (storage_iterator.elements_available())
    {
        auto expected_ip = expected.begin()[i++];
        auto actual_ip = storage_iterator.get_current();
        ASSERT_EQ(expected_ip, actual_ip);

        storage_iterator.move_next();
    }

    ASSERT_EQ(i, expected.size());
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

