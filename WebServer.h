#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

class WebServer
{
private:
    bool isBusy;
    int remainingTime;
    Request currentRequest;
public:
    WebServer();
    bool isIdle() const;
    void assignRequest(const Request& req);
    void process();
};
#endif