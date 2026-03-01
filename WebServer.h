#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

/**
 * @brief Represents a single web server in the load balancing system.
 * 
 * A WebServer processes incoming requests one at a time. Each server has a busy/idle state
 * and tracks the remaining processing time for its current request. Servers are managed by
 * LoadBalancer instances and contribute to overall system throughput.
 */
class WebServer
{
private:
    bool isBusy;              ///< Indicates whether the server is currently processing a request
    int remainingTime;        ///< Clock cycles remaining to complete the current request
    Request currentRequest;   ///< The request currently being processed
public:
    /**
     * @brief Constructs a new WebServer in an idle state.
     * 
     * Initializes the server with no active requests and zero remaining time.
     */
    WebServer();
    
    /**
     * @brief Checks if the server is available to accept new requests.
     * 
     * @return true if the server is idle and can accept a new request
     * @return false if the server is currently processing a request
     */
    bool isIdle() const;
    
    /**
     * @brief Assigns a new request to this server for processing.
     * 
     * Marks the server as busy and sets the remaining processing time based on
     * the request's time requirements. Should only be called when server is idle.
     * 
     * @param req The request to be processed by this server
     */
    void assignRequest(const Request& req);
    
    /**
     * @brief Processes the current request for one clock cycle.
     * 
     * Decrements the remaining time counter. When remaining time reaches zero,
     * the server automatically transitions back to idle state. This method should
     * be called once per simulation cycle.
     */
    void process();
};
#endif