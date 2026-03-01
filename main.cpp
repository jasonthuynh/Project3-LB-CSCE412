/**
 * @file main.cpp
 * @brief Entry point for the distributed load balancing simulation.
 * 
 * This program simulates a distributed web server infrastructure with intelligent
 * load balancing and automatic scaling. The simulation uses a Switch to route
 * requests between two specialized load balancers: one optimized for streaming
 * workloads and another for processing-intensive tasks.
 * 
 * Command-line arguments (all optional):
 * - argv[1]: Number of servers per load balancer (default: 10)
 * - argv[2]: Simulation duration in clock cycles (default: 10000)
 * - argv[3]: Scaling cooldown period in cycles (default: 200)
 * 
 * The simulation tracks performance metrics including throughput, request blocking,
 * task time distributions, and dynamic server scaling behavior. Results are logged
 * to separate files for each load balancer and displayed in color-coded console output.
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "LoadBalancer.h"
#include "Switch.h"

/**
 * @brief Main function executing the load balancing simulation.
 * 
 * Initializes the random number generator, parses command-line arguments,
 * creates two load balancer instances (streaming and processing), and runs
 * the simulation through a Switch coordinator.
 * 
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return int Exit status (0 for success)
 */
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

    LoadBalancer streamingLB(numServers, wait_n_cycles, "streaming_log.txt", 'S');
    LoadBalancer processingLB(numServers, wait_n_cycles, "processing_log.txt", 'P');

    streamingLB.generateInitialQueue();
    processingLB.generateInitialQueue();
    
    Switch networkSwitch(&streamingLB, &processingLB);
    networkSwitch.run(clockCycles, numServers);

    return 0;
}