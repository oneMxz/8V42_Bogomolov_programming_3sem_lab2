#include <gtest/gtest.h>
#include "ip_filter.h"

TEST(IPFilterTest, SplitFunction) {
    auto result = split("1.2.3.4", '.');
    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(result[0], "1");
    EXPECT_EQ(result[1], "2");
    EXPECT_EQ(result[2], "3");
    EXPECT_EQ(result[3], "4");
}

TEST(IPFilterTest, IPValidation) {
    EXPECT_TRUE(is_valid_ip("192.168.1.1"));
    EXPECT_FALSE(is_valid_ip("192.168.1")); // Не хватает октета
    EXPECT_FALSE(is_valid_ip("192.168.1.256")); // Байте > 255
    EXPECT_FALSE(is_valid_ip("192.168.1.a")); // Не число
}

TEST(IPFilterTest, ParseIP) {
    auto ip = parse_ip("192.168.1.1");
    EXPECT_EQ(std::get<0>(ip), 192);
    EXPECT_EQ(std::get<1>(ip), 168);
    EXPECT_EQ(std::get<2>(ip), 1);
    EXPECT_EQ(std::get<3>(ip), 1);
}

TEST(IPFilterTest, IntegrationTest) {
    IPPool ips = {
        {1,1,1,1},
        {1,2,1,1},
        {1,10,1,1},
        {46,70,1,1},
        {192,168,46,1}
    };
    
    sort_ips(ips);
    
    // Проверка обратной сортировки
    EXPECT_EQ(ips[0], std::make_tuple(192,168,46,1));
    EXPECT_EQ(ips[1], std::make_tuple(46,70,1,1));
    EXPECT_EQ(ips[2], std::make_tuple(1,10,1,1));
    EXPECT_EQ(ips[3], std::make_tuple(1,2,1,1));
    EXPECT_EQ(ips[4], std::make_tuple(1,1,1,1));
}

TEST(IPFilterTest, EdgeCases) {
    // Тестирование граничных значений
    EXPECT_TRUE(is_valid_ip("0.0.0.0"));
    EXPECT_TRUE(is_valid_ip("255.255.255.255"));
    EXPECT_FALSE(is_valid_ip("256.255.255.255"));
    EXPECT_FALSE(is_valid_ip("255.255.255.256"));
}

TEST(IPFilterTest, ReverseLexicographicSort) {
    IPPool ips = {
        {1, 1, 1, 1},
        {1, 2, 1, 1},
        {1, 10, 1, 1}
    };
    
    sort_ips(ips);
    
    EXPECT_EQ(ips[0], std::make_tuple(1, 10, 1, 1));
    EXPECT_EQ(ips[1], std::make_tuple(1, 2, 1, 1));
    EXPECT_EQ(ips[2], std::make_tuple(1, 1, 1, 1));
}

TEST(IPFilterTest, FilterByFirstByte) {
    IPPool ips = {
        {1, 1, 1, 1},
        {2, 1, 1, 1},
        {1, 2, 1, 1}
    };
    
    auto filtered = filter_ips(ips, filters::first_byte(1));
    EXPECT_EQ(filtered.size(), 2);
    for (const auto& ip : filtered) {
        EXPECT_EQ(std::get<0>(ip), 1);
    }
}