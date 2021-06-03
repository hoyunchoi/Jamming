#pragma once

#include <string>
#include <tuple>

#include "stringFormat.hpp"
#include "CSV.hpp"

namespace Jamming {

const std::string networkDirectory = "data/Network/";

const std::string getNetworkName(const std::tuple<unsigned, unsigned long long, double, int>& t_prefix) {
    std::string fileName = "N" + std::to_string(std::get<0>(t_prefix));
    fileName += "L" + std::to_string(std::get<1>(t_prefix));
    fileName += "G" + to_stringWithPrecision(std::get<2>(t_prefix), 1);
    fileName += "-" + std::to_string(std::get<3>(t_prefix));

    return fileName;
}

const std::string getDynamicsName(const std::tuple<double, unsigned, unsigned, int>& t_prefix) {
    std::string fileName = "S" + to_stringWithPrecision(std::get<0>(t_prefix), 2);
    fileName += "P" + std::to_string(std::get<1>(t_prefix));
    fileName += "I" + std::to_string(std::get<2>(t_prefix));
    fileName += "-" + std::to_string(std::get<3>(t_prefix));

    return fileName;
}



}