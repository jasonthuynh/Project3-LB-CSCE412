#include "LoadBalancer.h"
#include <iostream>
#include <sstream>

LoadBalancer::LoadBalancer(int numServers, int coolDown) {
    currentTime = 0;
    coolDownCounter = 0;
    coolDownPeriod = coolDown;

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
}

void LoadBalancer::generateInitialQueue() {
    int initSize = 100 * webservers.size();
    for (int i = 0; i < initSize; ++i) {
        Request r;
        if (!isBlockedIP(r.ipIn)) {
            requestQueue.push(r);
        } else {
            totalBlocked++;
        }
    }
}

void LoadBalancer::generateRandomRequests() {
    if (rand() % 100 < 30) { // 30% chance of new request
        Request r;
        if (!isBlockedIP(r.ipIn)) {
            requestQueue.push(r);
        } else {
            totalBlocked++;
        }
    }
}

void LoadBalancer::distributeRequests() {
    for (auto webserver: webservers) {
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
        std::cout << "Server added. Total servers: " << webservers.size() << "\n";
    } else if (queueSize < minThreshold * serverCount && serverCount > 1) {
        removeServer();
        coolDownCounter = coolDownPeriod;
        std::cout << "Server removed. Total servers: " << webservers.size() << "\n";
    }
}

void LoadBalancer::addServer() {
    webservers.push_back(new WebServer());
}

void LoadBalancer::removeServer() {
    for (int i = webservers.size() - 1; i >= 0; i--) {
        if (webservers[i]->isIdle()) {
            delete webservers[i];
            webservers.erase(webservers.begin() + i);
            return;
        }
    }
}

bool LoadBalancer::isBlockedIP(const std::string& ip) {
    // We are blocking 192.168.0.0 - 192.168.0.255
    if (ip.find("192.168.0.") == 0) {
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

    std::cout << "\nSimulation Complete\n";
    std::cout << "Total Processed: " << totalProcessed << "\n";
    std::cout << "Total Dropped (Firewall): " << totalBlocked << "\n";
}