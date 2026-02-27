#ifndef LOADBALANCER_H
#define LOADBALANCER_H
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

#include <vector>
#include <queue>
#include <fstream>
#include "Request.h"
#include "WebServer.h"

class LoadBalancer
{
private:
    std::vector<WebServer*> webservers;
    std::queue<Request> requestQueue;
    std::ofstream logFile;
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
    void logEvent(const std::string& message);
    void printSummary();
};

#endif