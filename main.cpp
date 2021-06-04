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

int main(int argc, char* argv[]) {
    //* ------------------------------------ Get input values ---------------------------------------
    //* Network structure parameters
    const unsigned networkSize = 10000;
    const unsigned long long linkSize = 25000;
    const double degreeExponent = 2.2;
    const int networkSeed = 0;
    pcg32 networkEngine(networkSeed);

    //* Jamming dynamics parameters
    const double strategy = std::stod(argv[1]);
    const unsigned newPackets = std::stoul(argv[2]);
    const unsigned maxIteration = 10000;
    const unsigned timeWindow = 10;
    const int generatorSeed = 0;
    if (timeWindow >= maxIteration){
        std::cout << "Time Window is bigger than max iteration\n";
        return 1;
    }

    //* ------------------------------------------------------------------------------------------------

    //* --------------------------------------- Default values -----------------------------------------

    //* Data path
    const std::string networkPrefix = Jamming::getNetworkPrefix(std::make_tuple(networkSize,
                                                                                linkSize,
                                                                                degreeExponent,
                                                                                networkSeed));
    const std::string dataDir = "data/" + networkPrefix + "/";
    const std::string adjName = "Adjacency.csv";
    const std::string distanceName = "FullDistance.csv";
    CSV::generateDirectory(dataDir);

    //* ------------------------------------------------------------------------------------------------

    //* ------------------- Generate scale free network and save information --------------------------
    // const auto startNetworkGeneration = std::chrono::system_clock::now();
    // const Network<unsigned> network = CL::generate(networkSize, linkSize, degreeExponent, networkEngine);
    // const std::vector<std::vector<unsigned>> fullDistance = network.getFullDistance();
    // network.printAdjacency(dataDir + adjName);
    // CSV::write(dataDir + distanceName, fullDistance);
    // const auto endNetworkGeneration = std::chrono::system_clock::now();
    // std::chrono::duration<double> networkGeneration = endNetworkGeneration - startNetworkGeneration;
    // std::cout << "Network Generation: " << std::setprecision(6) << networkGeneration.count() << " seconds\n";
    //* ------------------------------------------------------------------------------------------------

    //* ----------------------------------- Load scale free network ------------------------------------
    const auto startNetworkLoading = std::chrono::system_clock::now();
    Network<unsigned> network;
    std::vector<std::vector<unsigned>> fullDistance;
    network.loadAdjacency(dataDir + adjName);
    CSV::read(dataDir + distanceName, fullDistance);
    const auto endNetworkLoading = std::chrono::system_clock::now();
    std::chrono::duration<double> networkLoading = endNetworkLoading - startNetworkLoading;
    std::cout << "Network Loading: " << std::setprecision(6) << networkLoading.count() << " seconds\n";
    //* ------------------------------------------------------------------------------------------------

    //* ------------------------------------------- Single run -----------------------------------------
    const auto startGeneration = std::chrono::system_clock::now();
    Jamming::Generator generator(network, fullDistance, strategy, newPackets, timeWindow, generatorSeed);
    generator.run(maxIteration);
    generator.save(dataDir);
    const auto endGeneration = std::chrono::system_clock::now();
    std::chrono::duration<double> generation = endGeneration - startGeneration;
    std::cout << "Generator run for " << maxIteration << " times: " << std::setprecision(6) << generation.count() << " seconds\n";
    //* ------------------------------------------------------------------------------------------------

    //* ------------------------------------------- Multiple run -----------------------------------------
    // const auto startGeneration = std::chrono::system_clock::now();
    // const std::vector<unsigned> packetList = linearAlgebra::arange(newPackets, newPackets+10, (unsigned)1);
    // Jamming::Generator generator(network, fullDistance, strategy, newPackets, timeWindow, generatorSeed);
    // generator.multipleRun(maxIteration, packetList, dataDir);
    // const auto endGeneration = std::chrono::system_clock::now();
    // std::chrono::duration<double> generation = endGeneration - startGeneration;
    // std::cout << "Generator run for 10 ensembles " << maxIteration << " times: " << std::setprecision(6) << generation.count() << " seconds\n";
    //* ------------------------------------------------------------------------------------------------




    return 0;
}
