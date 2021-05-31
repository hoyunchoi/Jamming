
#include "Jamming.hpp"
#include "Networks.hpp"


int main(){
    Network<unsigned> network;
    pcg32 randomEngine;
    network = ER::generate((unsigned)10, (unsigned long long)10, randomEngine);

    const unsigned maxIteration = 100;

    Jamming model(network, 0.9, 2, randomEngine);
    model.run(maxIteration);

    return 0;
}