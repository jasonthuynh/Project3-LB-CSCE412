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

void Switch::run(int totalCycles) {
    for (int i = 0; i < totalCycles; i++) {
        if (rand() % 100 < 40) { // 40% chance of new request
            Request r;
            routeRequest(r);
        }
        streamingLB->runOneCycle();
        processingLB->runOneCycle();
    }
    streamingLB->printSummary(totalCycles);
    processingLB->printSummary(totalCycles);
}