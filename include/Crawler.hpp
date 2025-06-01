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

/**
 * @class Crawler
 * @brief A multithreaded web crawler that fetches and processes URLs starting from a base URL.
 */
class Crawler {
public:
    /**
     * @brief Constructs a Crawler with the given base URL, max visit limit, and batch fetch size.
     * 
     * @param base_url The root URL used to construct full URLs.
     * @param max_visit The maximum number of pages to visit.
     * @param batch_fetch_size The number of pages to fetch in a batch.
     */
    explicit Crawler(const std::string& base_url, size_t max_visit, int batch_fetch_size);

    /**
     * @brief Constructs a Crawler with the given base URL and max visit limit.
     * 
     * @param base_url The root URL used to construct full URLs.
     * @param max_visit The maximum number of pages to visit.
     */
    explicit Crawler(const std::string& base_url, size_t max_visit);

    /**
     * @brief Starts single-threaded crawling from the given start path.
     * 
     * Performs a breadth-first search from the specified path until the maximum number
     * of pages is visited or no more pages are left to crawl.
     * 
     * @param start_path The initial path (relative to base_url) to start crawling from.
     */
    void crawl(const std::string& start_path);

    /**
     * @brief Starts multi-threaded crawling using the specified number of threads.
     * 
     * @param start_path The initial path (relative to base_url) to start crawling from.
     * @param num_threads The number of worker threads to use.
     */
    void multi_crawl(const std::string& start_path, size_t num_threads);

    /**
     * @brief Crawls the graph and computes depths of all reachable pages from start_path.
     * 
     * @param start_path The starting page (relative to base_url).
     */
    void find_depths(const std::string& start_path);

    /// Adjacency list representation of the crawl graph.
    std::unordered_map<std::string, std::unordered_set<std::string>> graph;

    /// Thread-safe set of visited URLs.
    RefinableHashSet visited;

protected:
    /// The base/root URL used to construct full links.
    std::string base_url;

    /// Thread-safe queue of pages to visit.
    SafeUnboundedQueue to_visit;

    /// HTTP client used to fetch pages.
    HttpClient http;

    /// Number of pages to fetch in each batch (used in some implementations).
    int batch_fetch_size;

    /**
     * @brief Visits a single URL and extracts links from its content.
     * 
     * Sends an HTTP GET request to the given URL, parses the HTML content,
     * and extracts all valid links on the page.
     * 
     * @param url The full URL to visit.
     * @return A vector of discovered links on the page.
     */
    std::vector<std::string> visit(const std::string& url);

private:
    /// Maximum number of pages to visit.
    size_t max_visit;

    /// Atomic counter tracking the number of visited pages.
    std::atomic<size_t> num_visited;
};
