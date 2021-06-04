#pragma once

#include <string>
#include <tuple>

#include "CSV.hpp"
#include "stringFormat.hpp"

namespace Jamming {

const std::string getNetworkPrefix(const std::tuple<unsigned, unsigned long long, double, int>& t_prefix) {
    std::string prefix = "N" + std::to_string(std::get<0>(t_prefix));
    prefix += "L" + std::to_string(std::get<1>(t_prefix));
    prefix += "G" + to_stringWithPrecision(std::get<2>(t_prefix), 1);
    const int randomEngineSeed = std::get<3>(t_prefix);
    if (randomEngineSeed == -1) {
        return prefix;
    } else {
        return prefix + "-" + std::to_string(std::get<3>(t_prefix));
    }
}

const std::string getDynamicsPrefix(const std::tuple<double, unsigned, unsigned, int>& t_prefix) {
    std::string prefix = "S" + to_stringWithPrecision(std::get<0>(t_prefix), 2);
    prefix += "P" + std::to_string(std::get<1>(t_prefix));
    prefix += "I" + std::to_string(std::get<2>(t_prefix));
    const int randomEngineSeed = std::get<3>(t_prefix);
    if (randomEngineSeed == -1) {
        return prefix;
    } else {
        return prefix + "-" + std::to_string(std::get<3>(t_prefix));
    }
}

const std::string getDynamicsPrefixWOPacket(const std::tuple<double, unsigned, int>& t_prefix) {
    std::string prefix = "S" + to_stringWithPrecision(std::get<0>(t_prefix), 2);
    prefix += "I" + std::to_string(std::get<1>(t_prefix));
    const int randomEngineSeed = std::get<2>(t_prefix);
    if (randomEngineSeed == -1) {
        return prefix;
    } else {
        return prefix + "-" + std::to_string(std::get<2>(t_prefix));
    }
}

} // namespace Jamming