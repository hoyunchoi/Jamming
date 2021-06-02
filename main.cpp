#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <tuple>

#include "Networks.hpp"
#include "linearAlgebra.hpp"
#include "CSV.hpp"

#include "Generator.hpp"
#include "fileName.hpp"

int main() {
    //* ------------------------------------ Get input values ---------------------------------------
    //* Network structure parameters
    const unsigned networkSize = 10000;
    const unsigned long long linkSize = 200000;
    const double degreeExponent = 2.2;
    const int randomEngineSeed = 0;

    //* Jamming dynamics parameters
    const unsigned maxIteration = 100;
    const double strategy = 1.0;
    const unsigned newPackets = 3;
    //* ------------------------------------------------------------------------------------------------

    //* --------------------------------------- Default values -----------------------------------------
    //* Random engine
    pcg32 randomEngine(randomEngineSeed);

    //* Data path
    const std::string prefix = Jamming::getFileName(std::make_tuple(networkSize,
                                                                    linkSize,
                                                                    degreeExponent,
                                                                    randomEngineSeed));
    const std::string adjName = "Adj_" + prefix;
    const std::string degreeName = "Degree_" + prefix;
    const std::string distanceName = "Distance_" + prefix;
    //* ------------------------------------------------------------------------------------------------

    //* ------------------- Generate scale free network and save information --------------------------
    const auto startNetworkGeneration = std::chrono::system_clock::now();
    const Network<unsigned> network = CL::generate(networkSize, linkSize, degreeExponent, randomEngine);
    const std::vector<std::vector<unsigned>> fullDistance = network.getFullDistance();
    network.printAdjacency(Jamming::networkDirectory + adjName);
    network.printDegreeDist(Jamming::networkDirectory + degreeName);
    CSV::write(Jamming::networkDirectory + distanceName, fullDistance);
    const auto endNetworkGeneration = std::chrono::system_clock::now();
    std::chrono::duration<double> networkGeneration = startNetworkGeneration - endNetworkGeneration;
    std::cout << "Network Generation: " << std::setprecision(6) << networkGeneration.count() << " seconds\n";
    //* ------------------------------------------------------------------------------------------------

    //* ----------------------------------- Load scale free network ------------------------------------
    // const auto startNetworkLoading = std::chrono::system_clock::now();
    // Network<unsigned> network;
    // std::vector<std::vector<unsigned>> fullDistance;
    // network.loadAdjacency(Jamming::networkDirectory + adjName);
    // const auto endNetworkLoading = std::chrono::system_clock::now();
    // std::chrono::duration<double> networkLoading = startNetworkLoading - endNetworkLoading;
    // std::cout << "Network Loading: " << std::setprecision(6) << networkLoading.count() << " seconds\n";
    //* ------------------------------------------------------------------------------------------------

    //* ------------------------------------ Make Generator and run ------------------------------------
    // const auto startGeneration = std::chrono::system_clock::now();
    // Jamming::Generator generator(network, fullDistance, strategy, newPackets, randomEngine);
    // generator.run(maxIteration);
    // const auto endGeneration = std::chrono::system_clock::now();
    // std::chrono::duration<double> generation = startGeneration - endGeneration;
    // std::cout << "Network Loading: " << std::setprecision(6) << generation.count() << " seconds\n";
    //* ------------------------------------------------------------------------------------------------

    return 0;
}
