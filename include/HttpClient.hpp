#include <string>
#include <iostream>

/**
 * @class HttpClient
 * @brief A minimal HTTP client for fetching web page content.
 */
class HttpClient {
public:
    /**
     * @brief Sends an HTTP GET request to the specified URL.
     * 
     * This function performs an HTTP GET request to the given URL and returns
     * the response body as a string. It is used by the crawler to retrieve HTML content.
     * 
     * @param url The full URL to send the GET request to.
     * @return The response body as a string. Returns an empty string on failure.
     */
    std::string fetch(const std::string& url);
};
