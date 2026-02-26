#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <vector>
#include <queue>
#include "Request.h"
#include "WebServer.h"

class LoadBalancer
{
private:
    std::vector<WebServer*> webservers;
    std::queue<Request> requestQueue;
    int currentTime; // clock cycles
    int coolDownCounter;
    int coolDownPeriod;
    int totalProcessed;
    int totalBlocked;
    int minThreshold; // 50 * servers
    int maxThreshold; // 80 * servers

public:
    LoadBalancer(int numServers, int cooldown);
    ~LoadBalancer();

    void generateInitialQueue();
    void generateRandomRequests();
    void distributeRequests();
    void scaleServers();
    void run(int totalCycles);
    bool isBlockedIP(const std::string& ip);
    void addServer();
    void removeServer();
};

#endif