#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <tuple>

#include "CSV.hpp"
#include "Networks.hpp"

#include "fileName.hpp"

int main(int argc, char* argv[]) {
    //* Network structure parameters
    const unsigned networkSize = 10000;
    const unsigned long long linkSize = 25000;
    const double degreeExponent = 2.2;
    const int networkSeed = 0;
    pcg32 networkEngine(networkSeed);

    //* Data path
    const std::string networkPrefix = Jamming::getNetworkPrefix(std::make_tuple(networkSize,
                                                                                linkSize,
                                                                                degreeExponent,
                                                                                networkSeed));
    const std::string dataDir = "data/" + networkPrefix + "/";
    CSV::generateDirectory(dataDir);
    const std::string adjName = "Adjacency.csv";
    const std::string distanceName = "FullDistance.csv";

    //* Generate scale free network and calculate full distance
    const auto startNetworkGeneration = std::chrono::system_clock::now();
    const Network<unsigned> network = CL::generate(networkSize, linkSize, degreeExponent, networkEngine);
    const std::vector<std::vector<unsigned>> fullDistance = network.getFullDistance();

    //* Save
    network.printAdjacency(dataDir + adjName);
    CSV::write(dataDir + distanceName, fullDistance);

    //* Finished
    const auto endNetworkGeneration = std::chrono::system_clock::now();
    std::chrono::duration<double> networkGeneration = endNetworkGeneration - startNetworkGeneration;
    std::cout << networkPrefix << ": " << std::setprecision(6) << networkGeneration.count();

    return 0;
}
