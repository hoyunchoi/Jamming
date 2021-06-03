#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <tuple>

#include "CSV.hpp"
#include "Networks.hpp"
#include "linearAlgebra.hpp"

#include "Generator.hpp"
#include "fileName.hpp"

int main() {
    //* ------------------------------------ Get input values ---------------------------------------
    //* Network structure parameters
    const unsigned networkSize = 10000;
    const unsigned long long linkSize = 150000;
    const double degreeExponent = 2.2;
    const int networkSeed = 0;
    pcg32 networkEngine(networkSeed);

    //* Jamming dynamics parameters
    const double strategy = 0.85;
    const unsigned newPackets = 100;
    const unsigned maxIteration = 4000;
    const int generatorSeed = 0;
    pcg32 generatorEngine(generatorSeed);
    //* ------------------------------------------------------------------------------------------------

    //* --------------------------------------- Default values -----------------------------------------

    //* Data path
    const std::string networkPrefix = Jamming::getNetworkName(std::make_tuple(networkSize,
                                                                              linkSize,
                                                                              degreeExponent,
                                                                              networkSeed));
    // const std::string dynamicsPrefix = Jamming::getDynamicsName(std::make_tuple(strategy,
    //                                                                             newPackets,
    //                                                                             maxIteration,
    //                                                                             generatorSeed));
    const std::string adjName = "Adj_" + networkPrefix + ".csv";
    const std::string degreeName = "Degree_" + networkPrefix + ".csv";
    const std::string distanceName = "Distance_" + networkPrefix + ".csv";
    const std::string dataDirectory = "data/" + networkPrefix + "/";
    CSV::generateDirectory(dataDirectory);

    //* ------------------------------------------------------------------------------------------------

    //* ------------------- Generate scale free network and save information --------------------------
    // const auto startNetworkGeneration = std::chrono::system_clock::now();
    // const Network<unsigned> network = CL::generate(networkSize, linkSize, degreeExponent, networkEngine);
    // const std::vector<std::vector<unsigned>> fullDistance = network.getFullDistance();
    // network.printAdjacency(Jamming::networkDirectory + adjName);
    // network.printDegreeDist(Jamming::networkDirectory + degreeName);
    // CSV::write(Jamming::networkDirectory + distanceName, fullDistance);
    // const auto endNetworkGeneration = std::chrono::system_clock::now();
    // std::chrono::duration<double> networkGeneration = endNetworkGeneration - startNetworkGeneration;
    // std::cout << "Network Generation: " << std::setprecision(6) << networkGeneration.count() << " seconds\n";
    //* ------------------------------------------------------------------------------------------------

    //* ----------------------------------- Load scale free network ------------------------------------
    const auto startNetworkLoading = std::chrono::system_clock::now();
    Network<unsigned> network;
    std::vector<std::vector<unsigned>> fullDistance;
    network.loadAdjacency(Jamming::networkDirectory + adjName);
    CSV::read(Jamming::networkDirectory + distanceName, fullDistance);
    const auto endNetworkLoading = std::chrono::system_clock::now();
    std::chrono::duration<double> networkLoading = endNetworkLoading - startNetworkLoading;
    std::cout << "Network Loading: " << std::setprecision(6) << networkLoading.count() << " seconds\n";
    //* ------------------------------------------------------------------------------------------------

    //* ------------------------------------ Make Generator and run ------------------------------------
    const auto startGeneration = std::chrono::system_clock::now();
    Jamming::Generator generator(network, fullDistance, strategy, newPackets, generatorEngine);
    generator.run(maxIteration);
    generator.save(dataDirectory);
    const auto endGeneration = std::chrono::system_clock::now();
    std::chrono::duration<double> generation = endGeneration - startGeneration;
    std::cout << "Generator run for " << maxIteration << " times: " << std::setprecision(6) << generation.count() << " seconds\n";
    //* ------------------------------------------------------------------------------------------------

    return 0;
}
