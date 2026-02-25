#include "WebServer.h"

WebServer::WebServer() {
    isBusy = false;
    remainingTime = 0;
}

bool WebServer::isIdle() const {
    return !isBusy;
}

void WebServer::assignRequest(const Request& req) {
    currentRequest = req;
    isBusy = true;
    remainingTime = req.timeRequired;
}

void WebServer::process() {
    if (isBusy) {
        remainingTime--;
        if (remainingTime <= 0) {
            isBusy = false;
        }
    }
}