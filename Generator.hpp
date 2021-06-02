#pragma once

#include <deque>
#include <limits>
#include <random>
#include <tuple>
#include <vector>

#include "CSV.hpp"
#include "Networks.hpp"
#include "Router.hpp"
#include "pcg_random.hpp"
#include "stringFormat.hpp"

namespace Jamming {
struct Generator {
    //* --------------------------- variables --------------------------------
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
    unsigned m_numActivePackets;
    std::vector<unsigned> m_numActiveHistory;

    //* --------------------------- functions --------------------------------
  public:
    //* Constructor
    Generator() {}
    Generator(const Network<unsigned>&,                  //* Network
              const std::vector<std::vector<unsigned>>&, //* Full distance of netowrk
              const double&,                             //* Strategy parameter
              const unsigned&,                           //* Number of new packets per unit time
              const pcg32&);                             //* Random Engine

    //* Run the generator
    const unsigned run(const unsigned&);

  protected:
    void getFullDistance(const std::vector<std::set<unsigned>>&);
    void generatePackets();
    void movePackets();
    const unsigned getNextLocation(const unsigned&, const unsigned&) const;
};

Generator::Generator(const Network<unsigned>& t_network,
                     const std::vector<std::vector<unsigned>>& t_fullDistance,
                     const double& t_strategy,
                     const unsigned& t_newPacket,
                     const pcg32& t_randomEngine)
    : m_fullDistance(t_fullDistance),
      m_strategy(t_strategy),
      m_newPacket(t_newPacket),
      m_randomEngine(t_randomEngine) {
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

    //* Initialize information of dynamics
    m_numActivePackets = 0;
}

const unsigned Generator::run(const unsigned& t_maxIteration) {
    m_numActiveHistory.reserve(t_maxIteration);

    for (unsigned iter = 0; iter < t_maxIteration; ++iter) {
        generatePackets();
        movePackets();
        m_numActiveHistory.emplace_back(m_numActivePackets);
    }
    return m_numActivePackets;
}

//* Generator new packets and store to model
void Generator::generatePackets() {
    //?
    // std::cout << "Generated new packets\n";

    //* Generator new packets for 'm_newPacket' times
    for (unsigned i = 0; i < m_newPacket; ++i) {
        //* Randomly choose initial location and destination
        const unsigned initial = m_nodeDistribution(m_randomEngine);
        const unsigned destination = m_nodeDistribution(m_randomEngine);

        //* Generator packet at the initial location
        Packet packet(destination);

        //* Save the packet to the model
        m_routers[initial].queue.emplace_back(packet);

        //?
        // std::cout << "Location: " << initial << "\tDestination: " <<
        // destination
        // << "\n";
    }
    m_numActivePackets += m_newPacket;
}

//* Return next location of packet according to 'm_strategy'
const unsigned Generator::getNextLocation(const unsigned& t_location,
                                          const unsigned& t_destination) const {
    //* Set default minimum cost as infinity
    unsigned next_location;
    double minimum_cost = std::numeric_limits<double>::max();

    //* Find minimum cost among every neighbor of current location
    for (const unsigned& neighbor : m_routers[t_location].neighbors) {
        //* If neighbor is final destination, go to the destination
        if (neighbor == t_destination) {
            return neighbor;
        }

        //* Otherwise, calculate cost and choose minimum cost neighbor
        const unsigned queue = m_routers[neighbor].queue.size();
        const unsigned distance = m_fullDistance[neighbor][t_destination];
        const double cost = m_strategy * distance + (1.0 - m_strategy) * queue;

        //* Found neighbor with minimum cost
        if (cost < minimum_cost) {
            next_location = neighbor;
            minimum_cost = cost;
        }
    }

    return next_location;
}

//* Move every active packets
void Generator::movePackets() {
    //?
    // std::cout << "Moving packets\n";

    //* Iterate over all routers
    const std::vector<Router> old_routers = m_routers;
    for (const Router& router : old_routers) {
        if (router.queue.size()) {
            const unsigned index = router.index;

            //* Get the next location of front packet
            const Packet p = router.queue.front();
            const unsigned nextIndex = getNextLocation(index, p.destination);

            //?
            // std::cout << "Packet at " << index << " with destination " <<
            // p.destination << " moved to " << nextIndex;

            //* Update queue of previous router
            m_routers[index].queue.pop_front();

            //* Remove packet if it reaches to its destination
            if (nextIndex == p.destination) {
                --m_numActivePackets;
                //?
                // std::cout << ": Reached to destination\n";
            }
            //* Otherwise, queue the 'new' router
            else {
                m_routers[nextIndex].queue.emplace_back(p);
                //?
                // std::cout << "\n";
            }
        }
    }
}

} // namespace Jamming
