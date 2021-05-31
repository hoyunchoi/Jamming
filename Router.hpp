#pragma once

#include "Networks.hpp"

struct Router : Node<unsigned>{
    //* Member variables
    unsigned queue;
    bool impermeable;

    Router(){}
    Router(const unsigned& t_index): Node(t_index){
        queue = 0;
        impermeable = false;
    }
};