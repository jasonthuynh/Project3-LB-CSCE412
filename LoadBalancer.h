#ifndef LOADBALANCER_H
#define LOADBALANCER_H
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define PURPLE "\033[35m"
#define ORANGE "\033[38;5;208m"
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
    char lbType;
    int upperTaskTime = 0;
    int lowerTaskTime = 1000;

public:
    LoadBalancer(int numServers, int cooldown, const std::string& logFileName, char loadBalancerType);
    ~LoadBalancer();

    void generateInitialQueue();
    void generateRandomRequests();
    void distributeRequests();
    void scaleServers();
    void run(int totalCycles);
    bool isBlockedIP(const std::string& ip);
    void addServer();
    bool removeServer();
    void logEvent(const std::string& message);
    void printSummary(int totalCycles);
    void addRequest(const Request& req);
    void runOneCycle();
    void printLBType();
};

#endif