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

class Crawler2 {
public:
    explicit Crawler2(const std::string& base_url, size_t max_visit);

    void link_fetcher();


    void link_processor();
    void crawl(const std::string& start_path);


    void multi_crawl(const std::string& start_path, size_t num_threads_fetch, size_t num_threads_process);
    std::unordered_map<std::string, std::unordered_set<std::string>> graph;

protected:
    std::string base_url;                      ///< The root URL used to construct full URLs.
    SafeUnboundedQueue to_fetch;               ///< Queue of URLs to be visited.
    SafeUnboundedQueue to_process;
    RefinableHashSet visited;                  ///< Set of already visited URLs to avoid duplication.
    HttpClient http;                           ///< HTTP client for sending requests and receiving responses.

    int batch_fetch_size = 20;

    std::vector<std::string> visit(const std::string& url);

private:
    size_t max_visit; 
    std::atomic<size_t> num_visited;
};
