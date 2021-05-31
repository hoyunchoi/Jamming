#pragma once

struct Packet{
    unsigned location;
    unsigned destination;


    Packet(){}
    Packet(const unsigned& t_initialLocation, const unsigned& t_destination) : location(t_initialLocation), destination(t_destination){}

};