#include "helper.hpp"

namespace lab {

bool valid_timestamp(const std::string& timestamp) {
    std::tm time_get;
    if (strptime(timestamp.c_str(), "%Y-%m-%d", &time_get) || strptime(timestamp.c_str(), "%Y-%m-%d %H:%M:%S", &time_get)) {
        return true;
    }
    return false;
}

const std::string& default_timestamp() {
    static std::string sample_time("2021-08-29 21:27:23");
    return sample_time;
}

const std::string& default_string() {
    static std::string sample_string("sample string");
    return sample_string;
}

} // namespace lab
