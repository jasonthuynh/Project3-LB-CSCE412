#include "LoadBalancer.h"
#include <iostream>
#include <sstream>

LoadBalancer::LoadBalancer(int numServers, int coolDown, const std::string& logFileName, char loadBalancerType) {
    logFile.open(logFileName);
    currentTime = 0;
    coolDownCounter = 0;
    coolDownPeriod = coolDown;
    lbType = loadBalancerType;

    totalProcessed = 0;
    totalBlocked = 0;

    minThreshold = 50;
    maxThreshold = 80;

    for (int i = 0; i < numServers; ++i) {
        webservers.push_back(new WebServer());
    }
}

LoadBalancer::~LoadBalancer() {
    for (auto webserver: webservers) {
        delete webserver;
    }
    logFile.close();
}

void LoadBalancer::generateInitialQueue() {
    int initSize = 100 * webservers.size(); // queue starts full (100 * number of servers)
    for (int i = 0; i < initSize; ++i) {
        Request r;
        if (lbType == 'S') {
            r.jobType = 'S';
        }
        else if (lbType == 'P') {
            r.jobType = 'P';
        }
        requestQueue.push(r);
    }
}

void LoadBalancer::generateRandomRequests() {
    if (rand() % 100 < 40) { // 40% chance of new request
        Request r;
        requestQueue.push(r);
    }
}

void LoadBalancer::distributeRequests() {
    for (auto webserver: webservers) {
        // remove blocked IP addresses
        while (!requestQueue.empty() && isBlockedIP(requestQueue.front().ipIn)) {
            requestQueue.pop();
            totalBlocked++;
        }
        if (webserver->isIdle() && !requestQueue.empty()) {
            webserver->assignRequest(requestQueue.front());
            requestQueue.pop();
            totalProcessed++;
        }
        webserver->process();
    }
}

void LoadBalancer::scaleServers() {
    if (coolDownCounter > 0) {
        coolDownCounter--;
        return;
    }

    int queueSize = requestQueue.size();
    int serverCount = webservers.size();

    if (queueSize > maxThreshold * serverCount) {
        addServer();
        coolDownCounter = coolDownPeriod;
        printLBType();
        std::cout << GREEN << "Server added." << RESET << "Total servers: " << webservers.size() << "\n";
        logEvent("Server added. Total servers: " + std::to_string(webservers.size()));
    } else if (queueSize < minThreshold * serverCount && serverCount > 1) {
        if (removeServer()) {
            coolDownCounter = coolDownPeriod;
            printLBType();
            std::cout << YELLOW << "Server removed." << RESET << "Total servers: " << webservers.size() << "\n";
            logEvent("Server removed. Total servers: " + std::to_string(webservers.size()));
        }
    }
}

void LoadBalancer::addServer() {
    webservers.push_back(new WebServer());
}

bool LoadBalancer::removeServer() {
    for (int i = webservers.size() - 1; i >= 0; i--) {
        if (webservers[i]->isIdle()) {
            delete webservers[i];
            webservers.erase(webservers.begin() + i);
            return true;
        }
    }
    return false;
}

bool LoadBalancer::isBlockedIP(const std::string& ip) {
    // We are blocking IP addresses that start with 10
    if (ip.find("10.") == 0) {
        printLBType();
        std::cout << RED << "Blocked IP: " << ip << RESET << "\n";
        logEvent("Blocked IP: " + ip);
        return true;
    }
    return false;
}

void LoadBalancer::run(int totalCycles) {
    generateInitialQueue();

    for (int i = 0; i < totalCycles; i++) {
        currentTime++;

        generateRandomRequests();
        distributeRequests();
        scaleServers();
    }
}

void LoadBalancer::logEvent(const std::string& message) {
    logFile << "[Time " << currentTime << "] " << message << "\n";
}

void LoadBalancer::printSummary() {
    if (lbType == 'S') {
        std::cout << "\n===== " << BLUE << "Streaming Load Balancer Summary" << RESET << " =====\n";
        logFile << "\n===== Streaming Load Balancer Summary =====\n";
    }
    else if (lbType == 'P') {
        std::cout << "\n===== " << PURPLE << "Processing Load Balancer Summary" << RESET << " =====\n";
        logFile << "\n===== Processing Load Balancer Summary =====\n";
    }
    
    std::cout << "Total Processed: " << totalProcessed << "\n";
    std::cout << "Total Blocked (Firewall): " << totalBlocked << "\n";
    std::cout << "Final Server Count: " << webservers.size() << "\n";

    logFile << "Total Processed: " << totalProcessed << "\n";
    logFile << "Total Blocked (Firewall): " << totalBlocked << "\n";
    logFile << "Final Server Count: " << webservers.size() << "\n";
}

void LoadBalancer::addRequest(const Request& req) {
    requestQueue.push(req);
}

void LoadBalancer::runOneCycle() {
    currentTime++;
    distributeRequests();
    scaleServers();
}

void LoadBalancer::printLBType() {
    if (lbType == 'S') {
        std::cout << BLUE << "Streaming: " << RESET;
    }
    else if (lbType == 'P') {
        std::cout << PURPLE << "Processing: " << RESET;
    }
}