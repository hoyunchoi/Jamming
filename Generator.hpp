#pragma once

#include <deque>
#include <limits>
#include <random>
#include <set>
#include <tuple>
#include <vector>

#include "CSV.hpp"
#include "Networks.hpp"
#include "Router.hpp"
#include "fileName.hpp"
#include "pcg_random.hpp"
#include "stringFormat.hpp"

namespace Jamming {
struct Generator {
    //* --------------------------- variables --------------------------------
  protected:
    //* Network structure
    unsigned m_networkSize;
    std::vector<Router> m_routers;
    std::set<unsigned> m_activeIndex;
    std::vector<std::vector<unsigned>> m_fullDistance;

    //* Dynamics parameters
    double m_strategy;
    double m_newPacket;
    unsigned m_maxIteration;
    unsigned m_timeWindow;

    //* Random variables
    int m_randomEngineSeed;
    pcg32 m_randomEngine;
    std::uniform_int_distribution<unsigned> m_nodeDistribution;

    //* Save the information of dynamics
    unsigned m_totPacket;
    std::vector<unsigned> obs_totPacketHistory;
    std::vector<std::vector<unsigned>> obs_routerQueue;
    std::vector<double> obs_orderParameter;

    //* --------------------------- functions --------------------------------
  public:
    //* Constructor
    Generator() {}
    Generator(const Network<unsigned>&,                  //* Network
              const std::vector<std::vector<unsigned>>&, //* Full distance of netowrk
              const double&,                             //* Strategy parameter
              const unsigned&,                           //* Number of new packets per unit time
              const unsigned& = 10,                      //* Time window for order parameter calculation
              const int& = 0);                           //* Random Engine Seed

    //* Run the generator
    void singleRun(const unsigned&);
    void multipleRun(const unsigned&, const unsigned&);

    //* Save
    void saveSingleRun(const std::string&) const;
    void saveMultipleRun(const std::string&) const;

  protected:
    void getFullDistance(const std::vector<std::set<unsigned>>&);
    void generatePackets();
    void movePackets();
    const double getOrderParameter() const;
    const unsigned getNextLocation(const unsigned&, const unsigned&) const;

  private:
    //* Remain network and hyper parameters. Clear dynamics information
    //* Keep obs_orderparameter
    void clear();
};

Generator::Generator(const Network<unsigned>& t_network,
                     const std::vector<std::vector<unsigned>>& t_fullDistance,
                     const double& t_strategy,
                     const unsigned& t_newPacket,
                     const unsigned& t_timeWindow,
                     const int& t_randomEngineSeed)
    : m_fullDistance(t_fullDistance),
      m_strategy(t_strategy),
      m_newPacket(t_newPacket),
      m_timeWindow(t_timeWindow),
      m_randomEngineSeed(t_randomEngineSeed) {
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
    m_randomEngineSeed == -1 ? m_randomEngine.seed((std::random_device())()) : m_randomEngine.seed(m_randomEngineSeed);

    //* Initialize information of dynamics
    m_totPacket = 0;
}

void Generator::clear() {
    //* Clear router queue
    for (const unsigned index : m_activeIndex){
        m_routers[index].queue.clear();
    }
    m_activeIndex.clear();

    //* Clear packets
    m_totPacket = 0;
    obs_totPacketHistory.clear();
    obs_routerQueue.clear();
}

void Generator::singleRun(const unsigned& t_maxIteration) {
    m_maxIteration = t_maxIteration;
    obs_totPacketHistory.assign(m_maxIteration, 0);
    obs_routerQueue.reserve(m_maxIteration);

    //* Run generator for max iteration time
    for (unsigned iter = 0; iter < m_maxIteration; ++iter) {
        generatePackets();
        movePackets();
        //! Total packet history
        {
            obs_totPacketHistory[iter] = m_totPacket;
        }

        //! Router queue
        {
            std::vector<unsigned> routerQueue(m_networkSize, 0);
            for (const unsigned index : m_activeIndex) {
                routerQueue[index] = m_routers[index].queue.size();
            }
            obs_routerQueue.emplace_back(routerQueue);
        }
    }
}

void Generator::multipleRun(const unsigned& t_maxIteration, const unsigned& t_ensembleSize) {
    m_maxIteration = t_maxIteration;
    obs_orderParameter.assign(t_ensembleSize, 0.0);

    //* Run ensemble size time
    for (unsigned ensemble = 0; ensemble < t_ensembleSize; ++ensemble) {
        clear();
        obs_totPacketHistory.assign(m_maxIteration, 0);
        for (unsigned iter = 0; iter < m_maxIteration; ++iter) {
            generatePackets();
            movePackets();
            //! Total packet history
            {
                obs_totPacketHistory[iter] = m_totPacket;
            }
        }
        //* Calculate order parameter
        obs_orderParameter[ensemble] = getOrderParameter();
    }
}

const double Generator::getOrderParameter() const {
    const double orderParameter = (double)(obs_totPacketHistory[m_maxIteration - 1] - obs_totPacketHistory[m_maxIteration - m_timeWindow - 1]) / (m_timeWindow * m_newPacket);
    return orderParameter;
}

void Generator::saveSingleRun(const std::string& t_dirPath) const {
    //* Prefix of dynamics
    const std::string dynamicsPrefix = getDynamicsPrefix(std::make_tuple(m_strategy,
                                                                         m_newPacket,
                                                                         m_maxIteration,
                                                                         m_randomEngineSeed));
    //! Total packet history
    {
        const std::string totPacketDir = t_dirPath + "totPacket/";
        CSV::generateDirectory(totPacketDir);
        CSV::write(totPacketDir + dynamicsPrefix + ".csv", obs_totPacketHistory);
    }

    //! Order parameter
    {
        const std::string orderParameterDir = t_dirPath + "orderParameter/";
        CSV::generateDirectory(orderParameterDir);
        std::ofstream writeFile(orderParameterDir + dynamicsPrefix + ".csv", std::ios_base::app);
        writeFile.precision(std::numeric_limits<double>::digits10 + 1);
        writeFile << getOrderParameter() << "\n";
    }

    //! Router queue
    {
        const std::string queDir = t_dirPath + "routerQueue/";
        CSV::generateDirectory(queDir);
        CSV::write(queDir + dynamicsPrefix + ".csv", obs_routerQueue);
    }
}

void Generator::saveMultipleRun(const std::string& t_dirPath) const {
    //* Prefix of dynamics
    const std::string dynamicsPrefix = getDynamicsPrefix(std::make_tuple(m_strategy,
                                                                         m_newPacket,
                                                                         m_maxIteration,
                                                                         m_randomEngineSeed));

    //! Order Parameter
    {
        const std::string orderParameterDir = t_dirPath + "orderParameter/";
        CSV::generateDirectory(orderParameterDir);
        CSV::write(orderParameterDir + dynamicsPrefix + ".csv", obs_orderParameter, -1);
    }
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
        m_activeIndex.emplace(initial);

        //?
        // std::cout << "Location: " << initial << "\tDestination: " <<
        // destination
        // << "\n";
    }
    m_totPacket += m_newPacket;
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

            //* Erase queue of previous router
            m_routers[index].queue.pop_front();
            if (m_routers[index].queue.size() == 0) {
                m_activeIndex.erase(index);
            }

            //* Remove packet if it reaches to its destination
            if (nextIndex == p.destination) {
                --m_totPacket;
                //?
                // std::cout << ": Reached to destination\n";
            }
            //* Otherwise, queue the 'new' router
            else {
                m_routers[nextIndex].queue.emplace_back(p);
                m_activeIndex.emplace(nextIndex);
                //?
                // std::cout << "\n";
            }
        }
    }
}

} // namespace Jamming
