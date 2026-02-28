#include "Request.h"
#include <cstdlib>
#include <ctime>

Request::Request() {
    ipIn = generateRandomIP();
    ipOut = generateRandomIP();
    timeRequired = generateRandomTime();
    jobType = generateRandomJobType();
}

std::string Request::generateRandomIP() {
    return std::to_string(std::rand() % 256) + "." +
        std::to_string(std::rand() % 256) + "." +
        std::to_string(std::rand() % 256) + "." +
        std::to_string(std::rand() % 256); 
}

int Request::generateRandomTime() {
    return std::rand() % 100 + 1; // takes up 1-100 cycles
}

char Request::generateRandomJobType() {
    int random = std::rand() % 10;
    if (random > 3) {
        return 'P';
    } else {
        return 'S';
    }
}