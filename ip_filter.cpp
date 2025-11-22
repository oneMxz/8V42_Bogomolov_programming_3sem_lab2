#include "ip_filter.h"
#include <algorithm>
#include <sstream>
#include <cctype>
#include <stdexcept>

std::vector<std::string> split(const std::string &str, char d) {
    std::vector<std::string> r;
    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    
    while(stop != std::string::npos) {
        r.push_back(str.substr(start, stop - start));
        start = stop + 1;
        stop = str.find_first_of(d, start);
    }
    r.push_back(str.substr(start));
    return r;
}

bool is_valid_ip(const std::string& ip_str) {
    auto parts = split(ip_str, '.');
    if (parts.size() != 4) return false;
    
    for (const auto& part : parts) {
        if (part.empty() || !std::all_of(part.begin(), part.end(), 
            [](unsigned char c) { return std::isdigit(c); })) {
            return false;
        }
        int num = std::stoi(part);
        if (num < 0 || num > 255) return false;
    }
    return true;
}

IPAddress parse_ip(const std::string& ip_str) {
    auto parts = split(ip_str, '.');
    return std::make_tuple(
        std::stoi(parts[0]),
        std::stoi(parts[1]),
        std::stoi(parts[2]),
        std::stoi(parts[3])
    );
}

std::string ip_to_string(const IPAddress& ip) {
    std::ostringstream oss;
    oss << std::get<0>(ip) << "." 
        << std::get<1>(ip) << "." 
        << std::get<2>(ip) << "." 
        << std::get<3>(ip);
    return oss.str();
}

void sort_ips(IPPool& ip_pool) {
    std::sort(ip_pool.begin(), ip_pool.end(), 
              [](const auto& a, const auto& b) { return a > b; });
}

IPPool filter_ips(const IPPool& ip_pool, std::function<bool(const IPAddress&)> predicate) {
    IPPool result;
    std::copy_if(ip_pool.begin(), ip_pool.end(), 
                 std::back_inserter(result), predicate);
    return result;
}

namespace filters {
    std::function<bool(const IPAddress&)> first_byte(int byte) {
        return [byte](const IPAddress& ip) { return std::get<0>(ip) == byte; };
    }
    
    std::function<bool(const IPAddress&)> first_and_second_byte(int first, int second) {
        return [first, second](const IPAddress& ip) { 
            return std::get<0>(ip) == first && std::get<1>(ip) == second; 
        };
    }
    
    std::function<bool(const IPAddress&)> any_byte(int byte) {
        return [byte](const IPAddress& ip) {
            return std::get<0>(ip) == byte || 
                   std::get<1>(ip) == byte || 
                   std::get<2>(ip) == byte || 
                   std::get<3>(ip) == byte;
        };
    }
}