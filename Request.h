#ifndef REQUEST_H
#define REQUEST_H

#include <string>

/**
 * @brief Represents a network request in the load balancing simulation.
 * 
 * This struct encapsulates all properties of an incoming request including
 * source/destination IP addresses, processing time requirements, and job classification.
 * Requests are generated randomly and routed to appropriate load balancers.
 */
struct Request {
    std::string ipIn;        ///< Source IP address of the request
    std::string ipOut;       ///< Destination IP address of the request
    int timeRequired;        ///< Processing time in clock cycles (1-100)
    char jobType;            ///< Job classification: 'S' for streaming, 'P' for processing

    /**
     * @brief Constructs a new Request with randomly generated properties.
     * 
     * Initializes all request fields using random generation methods.
     * IP addresses, processing time, and job type are all determined randomly.
     */
    Request();
    
    /**
     * @brief Generates a random IPv4 address.
     * 
     * @return std::string A randomly generated IP address in dotted decimal notation (e.g., "192.168.1.1")
     */
    std::string generateRandomIP();
    
    /**
     * @brief Generates a random processing time for the request.
     * 
     * @return int Processing time in clock cycles, ranging from 1 to 100
     */
    int generateRandomTime();
    
    /**
     * @brief Generates a random job type classification.
     * 
     * Randomly assigns the request as either a streaming ('S') or processing ('P') job.
     * Processing jobs have a 60% probability, streaming jobs have 40% probability.
     * 
     * @return char Either 'P' for processing or 'S' for streaming
     */
    char generateRandomJobType();
};
#endif