#ifndef HELPER_HPP__
#define HELPER_HPP__

#include <string>
#include <ctime>

namespace lab {

bool valid_timestamp(const std::string& timestamp);

template<typename T>
constexpr T default_value() {
    constexpr T val = 0;
    return val;
}

const std::string& default_timestamp();

const std::string& default_string();

}

#endif // HELPER_HPP__
