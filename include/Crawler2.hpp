#pragma once

#include "HttpClient.hpp"
#include "../include/CrawlerUtils.hpp"
#include "../include/Page.hpp"
#include "../include/SafeUnboundedQueue.hpp"
#include "../include/RefinableHashSet.hpp"

#include <string>
#include <queue>
#include <unordered_set>
#include <vector>
#include <set>

/**
 * @class Crawler2
 * @brief A multi-threaded crawler that separates link fetching and link processing.
 *
 * This crawler design improves scalability and responsiveness by using dedicated
 * thread pools for fetching pages and processing their contents.
 */
class Crawler2 {
public:
    /**
     * @brief Constructs a Crawler2 instance with the specified base URL and visit limit.
     * 
     * @param base_url The root URL used for resolving relative links.
     * @param max_visit Maximum number of pages to visit before stopping.
     */
    explicit Crawler2(const std::string& base_url, size_t max_visit);

    /**
     * @brief Worker thread function for fetching pages from URLs in the to_fetch queue.
     * 
     * Each thread repeatedly fetches pages and enqueues the result into the to_process queue.
     */
    void link_fetcher();

    /**
     * @brief Worker thread function for processing fetched pages from the to_process queue.
     * 
     * Each thread extracts links from the fetched pages and updates the crawl graph.
     */
    void link_processor();

    /**
     * @brief Starts a single-threaded crawl from the specified start path.
     * 
     * Performs a basic crawl for prototyping and debugging.
     * 
     * @param start_path The initial relative path to begin crawling from.
     */
    void crawl(const std::string& start_path);

    /**
     * @brief Starts a multi-threaded crawl with separate fetch and process stages.
     * 
     * @param start_path The initial relative path to begin crawling from.
     * @param num_threads_fetch Number of threads dedicated to fetching pages.
     * @param num_threads_process Number of threads dedicated to processing links.
     */
    void multi_crawl(const std::string& start_path, size_t num_threads_fetch, size_t num_threads_process);

    /**
     * @brief Builds the graph and computes depths of all reachable pages from a start page.
     * 
     * @param start_path The initial page to compute depths from.
     */
    void find_depths(const std::string& start_path);

    /// Adjacency list representing the link graph discovered during crawling.
    std::unordered_map<std::string, std::unordered_set<std::string>> graph;

protected:
    /// The base/root URL used to resolve relative URLs.
    std::string base_url;

    /// Queue of URLs waiting to be fetched (input to the fetcher).
    SafeUnboundedQueue to_fetch;

    /// Queue of fetched content waiting to be processed (input to the processor).
    SafeUnboundedQueue to_process;

    /// Thread-safe set of already visited URLs to prevent redundant fetches.
    RefinableHashSet visited;

    /// HTTP client used for sending GET requests and receiving page content.
    HttpClient http;

    /// Number of pages to fetch per batch (used internally).
    const int batch_fetch_size = 20;

    /**
     * @brief Sends an HTTP GET request to a URL and extracts valid links.
     * 
     * @param url The full URL to fetch and parse.
     * @return A vector of discovered valid links on the page.
     */
    std::vector<std::string> visit(const std::string& url);

private:
    /// Maximum number of pages allowed to be visited.
    const size_t max_visit;

    /// Atomic counter of how many pages have been visited so far.
    std::atomic<size_t> num_visited;
};
