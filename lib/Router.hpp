#pragma once

#include "Networks.hpp"
#include <deque>

namespace Jamming {

struct Packet {
    unsigned destination;

    Packet() {}
    Packet(const unsigned& t_destination) : destination(t_destination) {}
};

struct Router : Node<unsigned> {
    //* Member variables
    std::deque<Packet> queue;
    bool impermeable;

    Router() {}
    Router(const unsigned& t_index) : Node(t_index) {
        impermeable = false;
    }
};

} // namespace Jamming
