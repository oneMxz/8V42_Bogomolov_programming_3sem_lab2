#ifndef IP_FILTER_H
#define IP_FILTER_H

#include <vector>
#include <string>
#include <tuple>
#include <functional>

using IPAddress = std::tuple<int, int, int, int>;
using IPPool = std::vector<IPAddress>;

// Парсинг и валидация
IPAddress parse_ip(const std::string& ip_str);
bool is_valid_ip(const std::string& ip_str);

// Утилиты
std::vector<std::string> split(const std::string &str, char d);
std::string ip_to_string(const IPAddress& ip);

// Основные операции
void sort_ips(IPPool& ip_pool);
IPPool filter_ips(const IPPool& ip_pool, std::function<bool(const IPAddress&)> predicate);

// Фабрика предикатов для фильтрации
namespace filters {
    std::function<bool(const IPAddress&)> first_byte(int byte);
    std::function<bool(const IPAddress&)> first_and_second_byte(int first, int second);
    std::function<bool(const IPAddress&)> any_byte(int byte);
}

#endif