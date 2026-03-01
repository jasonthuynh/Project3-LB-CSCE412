#ifndef SWITCH_H
#define SWITCH_H

#include "LoadBalancer.h"
#include "Request.h"

/**
 * @brief Orchestrates request routing between specialized load balancers.
 * 
 * The Switch acts as a central router that manages two separate load balancers:
 * one optimized for streaming workloads and another for processing workloads.
 * It generates incoming requests, classifies them by job type, and routes them
 * to the appropriate load balancer. The Switch also coordinates the simulation
 * execution across both load balancers.
 */
class Switch {
    private:
        LoadBalancer* streamingLB;    ///< Load balancer dedicated to streaming requests ('S' jobs)
        LoadBalancer* processingLB;   ///< Load balancer dedicated to processing requests ('P' jobs)

    public:
        /**
         * @brief Constructs a new Switch with two load balancer instances.
         * 
         * @param streamLB Pointer to the load balancer handling streaming requests
         * @param processLB Pointer to the load balancer handling processing requests
         */
        Switch(LoadBalancer* streamLB, LoadBalancer* processLB);
        
        /**
         * @brief Routes a request to the appropriate load balancer based on job type.
         * 
         * Examines the request's jobType field and forwards it to either the
         * streaming load balancer ('S') or the processing load balancer ('P').
         * 
         * @param req The request to be routed
         */
        void routeRequest(const Request& req);
        
        /**
         * @brief Executes the complete load balancing simulation.
         * 
         * Runs the simulation for the specified number of clock cycles. Each cycle:
         * - Randomly generates new requests (40% probability per cycle)
         * - Routes new requests to appropriate load balancers
         * - Advances both load balancers by one cycle
         * 
         * After completion, prints performance summaries for both load balancers
         * including throughput, blocked requests, and server scaling metrics.
         * 
         * @param totalCycles Number of clock cycles to run the simulation
         */
        void run(int totalCycles);
};

#endif