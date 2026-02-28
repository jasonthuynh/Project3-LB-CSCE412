#ifndef SWITCH_H
#define SWITCH_H

#include "LoadBalancer.h"
#include "Request.h"

class Switch {
    private:
        LoadBalancer* streamingLB;
        LoadBalancer* processingLB;

    public:
        Switch(LoadBalancer* streamLB, LoadBalancer* processLB);
        void routeRequest(const Request& req);
        void run(int totalCycles);
};

#endif