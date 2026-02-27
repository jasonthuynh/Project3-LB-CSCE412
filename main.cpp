#include <iostream>
#include <cstdlib>
#include <ctime>
#include "LoadBalancer.h"

int main() {
    srand(time(0));
    
    int numServers;
    int simulationTime;

    std::cout << "Enter number of servers: ";
    std::cin >> numServers;

    std::cout << "Enter simulation time (clock cycles): ";
    std::cin >> simulationTime;

    LoadBalancer lb(numServers, 50);
    lb.run(simulationTime);

    return 0;
}