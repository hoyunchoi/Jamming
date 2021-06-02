#pragma once

#include <string>
#include <tuple>

#include "stringFormat.hpp"

namespace Jamming {

const std::string networkDirectory = "data/Network/";

const std::string getFileName(const std::tuple<unsigned, unsigned long long, double, int>& t_prefix) {
    std::string fileName = "N" + std::to_string(std::get<0>(t_prefix));
    fileName += "L" + std::to_string(std::get<1>(t_prefix));
    fileName += "G" + to_stringWithPrecision(std::get<2>(t_prefix), 1);
    fileName += "-" + std::to_string(std::get<3>(t_prefix));

    return fileName;
}



}