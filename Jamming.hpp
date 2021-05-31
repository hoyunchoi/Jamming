#pragma once

#include <limits>
#include <random>
#include <vector>

#include "Networks.hpp"
#include "Packet.hpp"
#include "Router.hpp"
#include "pcg_random.hpp"

struct Jamming {
    //* Member variables
   protected:
    //* Network structure
    unsigned m_networkSize;
    std::vector<Router> m_routers;
    std::vector<std::vector<unsigned>> m_fullDistance;

    //* Dynamics parameters
    double m_strategy;
    double m_newPacket;

    //* Random variables
    pcg32 m_randomEngine;
    std::uniform_int_distribution<unsigned> m_nodeDistribution;

    //* Save the information of dynamics
    std::vector<Packet> m_activePackets;
    std::vector<unsigned> m_numActive;

    //* Member functions
   public:
    Jamming() {}
    Jamming(const Network<unsigned>&, const double&, const unsigned&, const pcg32&);

    const unsigned run(const unsigned&);

   protected:
    void getFullDistance();
    const unsigned getDistance(const unsigned&, const unsigned&) const;
    void generatePacket();
    void movePacket();
    const unsigned getNextLocation(const unsigned&, const unsigned&) const;
};

//* Initialize
Jamming::Jamming(const Network<unsigned>& t_network, const double& t_strategy, const unsigned& t_newPacket, const pcg32& t_randomEngine) : m_strategy(t_strategy), m_newPacket(t_newPacket), m_randomEngine(t_randomEngine) {
    //* Get network information
    m_networkSize = t_network.size;
    m_routers.reserve(m_networkSize);
    for (unsigned index = 0; index < m_networkSize; ++index) {
        Router router(index);
        router.neighbors = t_network.adjacency[index];
        m_routers.emplace_back(router);
    }

    //* Random variables
    m_nodeDistribution.param(std::uniform_int_distribution<unsigned>::param_type(0, m_networkSize - 1));
}

const unsigned Jamming::run(const unsigned& t_maxIteration) {
    m_numActive.reserve(t_maxIteration);

    for (unsigned iter = 0; iter < t_maxIteration; ++iter) {
        generatePacket();
        movePacket();
        m_numActive.emplace_back(m_activePackets.size());
    }
    return m_activePackets.size();
}

void Jamming::getFullDistance() {
    //* Assign default value as infinity (maximum value of unsinged)
    m_fullDistance.assign(m_networkSize, std::vector<unsigned>(m_networkSize, -1));

    //* Store distance between every node pairs
    for (unsigned i = 0; i < m_networkSize; ++i) {
        for (unsigned j = i + 1; j < m_networkSize; ++j) {
            const unsigned distance = getDistance(i, j);
            m_fullDistance[i][j] = distance;
            m_fullDistance[j][i] = distance;
        }
    }
}

//* Calculate distance between two nodes
const unsigned Jamming::getDistance(const unsigned& t_index1, const unsigned& t_index2) const {
    unsigned distance = 0;

    return distance;
}

//* Generate new packets and store to model
void Jamming::generatePacket() {
    //* Generate new packets for 'm_newPacket' times
    for (unsigned i = 0; i < m_newPacket; ++i) {
        //* Randomly choose initial location and destination
        const unsigned initial = m_nodeDistribution(m_randomEngine);
        const unsigned destination = m_nodeDistribution(m_randomEngine);

        //* Generate packet at the initial location
        Packet packet(initial, destination);

        //* Save the packet to the model
        m_activePackets.emplace_back(packet);
    }
}

//* Return next location of packet according to 'm_strategy'
const unsigned Jamming::getNextLocation(const unsigned& t_location, const unsigned& t_destination) const {
    //* Set default minimum cost as infinity
    double minimum_cost = std::numeric_limits<double>::max();
    unsigned next_location = -1;

    //* Find minimum cost among every neighbor of current location
    for (const unsigned& neighbor : m_routers[t_location].neighbors) {
        const unsigned queue = m_routers[neighbor].queue;
        const unsigned distance = m_fullDistance[neighbor][t_destination];
        const double cost = m_strategy * distance + (1 - m_strategy) * queue;

        //* Found neighbor with minimum cost
        if (cost < minimum_cost) {
            next_location = neighbor;
            minimum_cost = cost;
        }
    }

    return next_location;
}

//* Move every active packets
void Jamming::movePacket() {
    //* Iterate over all active packets
    auto it = m_activePackets.begin();
    while (it != m_activePackets.end()) {
        //* Reduce queue of 'old' router
        m_routers[it->location].queue -= 1;

        //? Update location of packet according to strategy
        it->location = getNextLocation(it->location, it->destination);

        //* Increase queue of 'new' router
        m_routers[it->location].queue += 1;

        //* Remove packet if it reaches to its destination
        if (it->location == it->destination) {
            it = m_activePackets.erase(it);
        } else {
            ++it;
        }
    }
}
