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

/**
 * @brief Manages dynamic load distribution across a pool of web servers.
 * 
 * The LoadBalancer implements an intelligent request distribution system with automatic
 * scaling capabilities. It maintains a queue of incoming requests, distributes them to
 * available servers, and dynamically adds or removes servers based on current load.
 * The system includes firewall capabilities for blocking certain IP addresses and
 * comprehensive logging for performance analysis.
 * 
 * Key features:
 * - Dynamic server scaling based on queue thresholds
 * - IP-based firewall filtering
 * - Performance metrics tracking (throughput, task time ranges)
 * - Detailed event logging
 * - Support for specialized workload types (streaming vs. processing)
 */
class LoadBalancer
{
private:
    std::vector<WebServer*> webservers;  ///< Pool of managed web servers
    std::queue<Request> requestQueue;    ///< FIFO queue of pending requests
    std::ofstream logFile;               ///< Output file stream for event logging
    int currentTime;                     ///< Current simulation clock cycle
    int coolDownCounter;                 ///< Cycles remaining before next scaling operation
    int coolDownPeriod;                  ///< Minimum cycles between scaling operations
    int totalProcessed;                  ///< Total number of successfully processed requests
    int totalBlocked;                    ///< Total number of requests blocked by firewall
    int minThreshold;                    ///< Queue size threshold for removing servers (50 * serverCount)
    int maxThreshold;                    ///< Queue size threshold for adding servers (80 * serverCount)
    char lbType;                         ///< Load balancer type: 'S' for streaming, 'P' for processing
    int upperTaskTime;                   ///< Maximum task time encountered across all requests
    int lowerTaskTime;                   ///< Minimum task time encountered across all requests

public:
    /**
     * @brief Constructs a new LoadBalancer with specified configuration.
     * 
     * Initializes the load balancer with a pool of web servers, sets up logging,
     * and configures scaling thresholds. The cooldown period prevents rapid
     * oscillation during scaling operations.
     * 
     * @param numServers Initial number of web servers to create
     * @param cooldown Number of clock cycles to wait between scaling operations
     * @param logFileName Path to the log file for event recording
     * @param loadBalancerType Type identifier: 'S' for streaming, 'P' for processing
     */
    LoadBalancer(int numServers, int cooldown, const std::string& logFileName, char loadBalancerType);
    
    /**
     * @brief Destructor that cleans up all allocated web servers and closes log file.
     * 
     * Deallocates all dynamically created WebServer instances and ensures the
     * log file is properly flushed and closed.
     */
    ~LoadBalancer();

    /**
     * @brief Populates the request queue with initial workload.
     * 
     * Generates an initial batch of requests (100 per server) to simulate
     * an existing workload at simulation start. All requests are tagged with
     * the load balancer's job type. Updates task time range statistics and
     * logs the starting queue size.
     */
    void generateInitialQueue();
    
    /**
     * @brief Randomly generates new incoming requests during simulation.
     * 
     * Called once per clock cycle. Has a 30% probability of generating a new
     * request and adding it to the queue. Updates task time range statistics
     * for generated requests.
     */
    void generateRandomRequests();
    
    /**
     * @brief Distributes queued requests to available servers.
     * 
     * For each idle server, attempts to assign a request from the queue.
     * Automatically filters and blocks requests from blacklisted IP addresses
     * before assignment. Advances processing on all active servers by one cycle.
     * Updates totalProcessed and totalBlocked counters.
     */
    void distributeRequests();
    
    /**
     * @brief Dynamically scales server pool based on current load.
     * 
     * Evaluates queue size against threshold values and adds/removes servers
     * as needed. Scaling is subject to cooldown periods to prevent oscillation.
     * 
     * Scaling rules:
     * - Add server if: queueSize > maxThreshold * serverCount
     * - Remove server if: queueSize < minThreshold * serverCount AND serverCount > 1
     * 
     * Logs all scaling operations and resets cooldown counter after each change.
     */
    void scaleServers();
    
    /**
     * @brief Executes a complete simulation run.
     * 
     * Generates initial queue, then runs the simulation for the specified
     * number of cycles. Each cycle generates random requests, distributes
     * work to servers, and performs scaling checks.
     * 
     * Note: This method is not used when running with the Switch class.
     * 
     * @param totalCycles Number of clock cycles to simulate
     */
    void run(int totalCycles);
    
    /**
     * @brief Checks if an IP address should be blocked by the firewall.
     * 
     * Currently blocks all IP addresses beginning with "10." (private network range).
     * Logs blocked IPs and outputs colored warnings to console.
     * 
     * @param ip The IP address string to check
     * @return true if the IP should be blocked
     * @return false if the IP is allowed
     */
    bool isBlockedIP(const std::string& ip);
    
    /**
     * @brief Adds a new web server to the pool.
     * 
     * Dynamically allocates and adds a new WebServer instance to expand capacity.
     * Called by scaleServers() when load exceeds maximum threshold.
     * Logs the addition event.
     */
    void addServer();
    
    /**
     * @brief Removes an idle server from the pool.
     * 
     * Searches for an idle server and removes it to reduce capacity.
     * Only removes servers that are not currently processing requests.
     * Called by scaleServers() when load falls below minimum threshold.
     * 
     * @return true if a server was successfully removed
     * @return false if no idle servers were available for removal
     */
    bool removeServer();
    
    /**
     * @brief Records an event to the log file with timestamp.
     * 
     * Writes a timestamped message to the log file for audit and analysis purposes.
     * 
     * @param message The event description to log
     */
    void logEvent(const std::string& message);
    
    /**
     * @brief Prints comprehensive performance summary to console and log file.
     * 
     * Outputs detailed statistics including:
     * - Total requests processed
     * - Throughput as percentage of total cycles
     * - Number of firewall-blocked requests
     * - Task time range (min to max)
     * - Final server count
     * - Ending queue size
     * 
     * Output is color-coded based on load balancer type and written to both
     * console and log file.
     * 
     * @param totalCycles Total number of simulation cycles for throughput calculation
     */
    void printSummary(int totalCycles, int numServers);
    
    /**
     * @brief Adds a request directly to the queue (used by Switch).
     * 
     * Allows external components (like the Switch) to add requests to this
     * load balancer's queue without going through random generation.
     * 
     * @param req The request to add to the queue
     */
    void addRequest(const Request& req);
    
    /**
     * @brief Advances the simulation by one clock cycle.
     * 
     * Increments the simulation clock, distributes pending requests to servers,
     * and evaluates scaling needs. Called by the Switch during coordinated
     * multi-load-balancer simulations.
     */
    void runOneCycle();
    
    /**
     * @brief Prints the load balancer type identifier to console.
     * 
     * Outputs a color-coded prefix indicating whether this is a streaming ('S')
     * or processing ('P') load balancer. Used for distinguishing output in
     * multi-load-balancer configurations.
     */
    void printLBType();
};

#endif