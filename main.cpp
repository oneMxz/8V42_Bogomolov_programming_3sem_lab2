#include "ip_filter.h"
#include <iostream>


void print_ips(const IPPool& ip_pool) {
    for (const auto& ip : ip_pool) {
        std::cout << ip_to_string(ip) << std::endl;
    }
}

int main() {
    try {
        IPPool ip_pool;
        
        for (std::string line; std::getline(std::cin, line);) {
            auto fields = split(line, '\t');
            if (!fields.empty() && is_valid_ip(fields[0])) {
                ip_pool.push_back(parse_ip(fields[0]));
            }
        }
        // Сортировка
        sort_ips(ip_pool);
        
        // Вывод результатов
        std::cout << "All IPs sorted:" << std::endl;
        print_ips(ip_pool);
        
        std::cout << "\nIPs with first byte = 1:" << std::endl;
        print_ips(filter_ips(ip_pool, filters::first_byte(1)));
        
        std::cout << "\nIPs with 46.70.x.x:" << std::endl;
        print_ips(filter_ips(ip_pool, filters::first_and_second_byte(46, 70)));
        
        std::cout << "\nIPs with any byte = 46:" << std::endl;
        print_ips(filter_ips(ip_pool, filters::any_byte(46)));

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}