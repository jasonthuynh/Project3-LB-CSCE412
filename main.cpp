#include <iostream>
#include <cstdlib>
#include <ctime>
#include "LoadBalancer.h"

int main(int argc, char* argv[]) {
    srand(time(0));
    
    // Default vals
    int numServers = 10;
    int clockCycles = 10000;
    int wait_n_cycles = 200;

    if (argc > 1) {
        numServers = std::atoi(argv[1]);
    }
    if (argc > 2) {
        clockCycles = std::atoi(argv[2]);
    }
    if (argc > 3) {
        wait_n_cycles = std::atoi(argv[3]);
    }

    std::cout << "\n" << "Starting simulation with " << numServers << " servers for " << clockCycles << " clock cycles.\n\n";

    LoadBalancer lb(numServers, wait_n_cycles);
    lb.run(clockCycles);
    lb.printSummary();

    return 0;
}