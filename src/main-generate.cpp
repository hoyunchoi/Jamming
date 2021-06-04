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
    //* Network structure parameters
    const unsigned networkSize = 100;
    const unsigned long long linkSize = 250;
    const double degreeExponent = 2.2;
    const int networkSeed = 0;

    //* Jamming dynamics parameters
    const double strategy = std::stod(argv[1]);
    const unsigned newPackets = std::stoul(argv[2]);
    const unsigned newPacket_list_num = std::stoul(argv[3]);
    const int generatorSeed = -1;
    const bool saveQueue = false;

    //* Data path
    const std::string networkPrefix = Jamming::getNetworkPrefix(std::make_tuple(networkSize,
                                                                                linkSize,
                                                                                degreeExponent,
                                                                                networkSeed));
    const std::string dataDir = "data/" + networkPrefix + "/";
    const std::string adjName = "Adjacency.csv";
    const std::string distanceName = "FullDistance.csv";
    CSV::generateDirectory(dataDir);

    //* Load scale free network
    Network<unsigned> network;
    std::vector<std::vector<unsigned>> fullDistance;
    network.loadAdjacency(dataDir + adjName);
    CSV::read(dataDir + distanceName, fullDistance);

    //* Set packet list
    const auto startGeneration = std::chrono::system_clock::now();
    std::vector<unsigned> packetList = linearAlgebra::arange(newPackets,
                                                             newPackets + newPacket_list_num - 1,
                                                             (unsigned)1);

    //* Run generator
    Jamming::Generator generator(network, fullDistance, strategy, newPackets, saveQueue, generatorSeed);
    generator.run(packetList, dataDir);

    //* Finished
    const auto endGeneration = std::chrono::system_clock::now();
    std::chrono::duration<double> generation = endGeneration - startGeneration;
    std::cout << "S" << strategy << "P(" << packetList.front() << "->" << packetList.back() << "): " << std::setprecision(6) << generation.count() << "\n";

    return 0;
}
