#include "HttpClient.hpp"
#include "../include/CrawlerUtils.hpp"

#include <string>
#include <queue>
#include <unordered_set>
#include <vector>

/**
 * @class Crawler
 * @brief A simple web crawler that recursively fetches and processes URLs from a given base URL.
 */
class Crawler {
public:
    /**
     * @brief Constructs a Crawler object with a given base URL.
     * 
     * @param base_url The root URL to be used for relative link resolution.
     */
    explicit Crawler(const std::string& base_url);

    /**
     * @brief Starts crawling from the given start path.
     * 
     * This function performs a breadth-first search starting from the specified path.
     * It continues crawling new links found on each page until the `max_visit` limit is reached
     * or no more unvisited links are available.
     * 
     * @param start_path The initial path (relative to base_url) to start crawling from.
     * @param max_visit Maximum number of pages to visit. Defaults to 1000.
     */
    void crawl(const std::string& start_path, int max_visit = 1000);

private:
    std::string base_url;                      ///< The root URL used to construct full URLs.
    std::queue<std::string> to_visit;          ///< Queue of URLs to be visited.
    std::unordered_set<std::string> visited;   ///< Set of already visited URLs to avoid duplication.
    HttpClient http;                           ///< HTTP client for sending requests and receiving responses.

    /**
     * @brief Visits a single URL and extracts links from its content.
     * 
     * This function performs an HTTP GET request to the given URL and uses a utility
     * to parse and extract links from the HTML content. It returns a list of discovered URLs.
     * 
     * @param url The full URL to visit.
     * @return A vector of discovered links on the page.
     */
    std::vector<std::string> visit(const std::string& url);
};
