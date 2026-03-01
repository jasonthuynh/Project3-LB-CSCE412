/**
 * @file Switch.cpp
 * @brief Implementation of the Switch class for request routing.
 * 
 * Coordinates two load balancers by routing requests based on job type,
 * managing simulation execution, and aggregating performance results.
 */

#include "Switch.h"
#include <iostream>

Switch::Switch(LoadBalancer* streamLB, LoadBalancer* processLB) {
    streamingLB = streamLB;
    processingLB = processLB;
}

void Switch::routeRequest(const Request& req) {
    if (req.jobType == 'S') {
        streamingLB->addRequest(req);
    }
    else if (req.jobType == 'P') {
        processingLB->addRequest(req);
    }
}

void Switch::run(int totalCycles, int numServers) {
    for (int i = 0; i < totalCycles; i++) {
        if (rand() % 100 < 40) { // 40% chance of new request
            Request r;
            routeRequest(r);
        }
        streamingLB->runOneCycle();
        processingLB->runOneCycle();
    }
    streamingLB->printSummary(totalCycles, numServers);
    processingLB->printSummary(totalCycles, numServers);
}