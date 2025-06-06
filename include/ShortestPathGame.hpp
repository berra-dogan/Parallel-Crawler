#pragma once

#include <string>
#include <vector>
#include "Crawler2.hpp"
#include "SafeUnboundedQueue.hpp"
#include "RefinableHashSet.hpp"
#include "HttpClient.hpp"

/**
 * @class ShortestPathGame
 * @brief A multithreaded crawler that finds the shortest path between two URLs on a website.
 * 
 * Inherits from the `Crawler2` base class. The goal is to identify the shortest hyperlink path
 * from a given start page to a target end page by traversing the web graph starting from the
 * `start_path` using a breadth-first strategy.
 * 
 * If multiple solutions of the same shortest length exist, they are all stored.
 */
class ShortestPathGame : public Crawler2 {
public:
    /**
     * @brief Constructor for ShortestPathGame.
     * 
     * @param base_url The base domain to restrict the crawling (e.g., "https://example.com").
     * @param start_path The path (relative to base_url) where the search starts.
     * @param end_path The target path to find.
     * 
     * @throws std::invalid_argument if start_path and end_path are the same.
     */
    explicit ShortestPathGame(const std::string& base_url,
                              const std::string& start_path,
                              const std::string& end_path,
                              bool all_solutions = true
                            )
        : Crawler2(base_url, 0), start_path(start_path), end_path(end_path), all_solutions(all_solutions) {
        if (start_path == end_path) {
            throw std::invalid_argument("Start and end are the same");
        }
    }

    /**
     * @brief Launches multiple threads to perform parallel shortest path search.
     * 
     * @param num_threads Number of worker threads to use in the search.
     */
    void multi_find(size_t num_threads);

    /**
     * @brief Performs a single-threaded portion of the BFS crawl.
     * 
     * This function is called by each thread during `multi_find`.
     * 
     * @param active_threads Atomic counter to track active worker threads.
     */
    void find(std::atomic<size_t>& active_threads);

private:
    std::string start_path;  ///< Relative path to the start page (e.g., "/wiki/A").
    std::string end_path;    ///< Relative path to the end page (e.g., "/wiki/B").
    bool all_solutions;      ///whether to find all shortest path solutions or only 1

    /**
     * @brief Extracts all valid path solutions from the visited graph.
     * 
     * Called once the BFS completes to gather all paths that match the shortest depth.
     */
    void find_path_solutions();

    std::atomic<size_t> best_depth{std::numeric_limits<size_t>::max()};  ///< Tracks the shortest depth discovered so far.

    std::vector<std::vector<std::string>> solutions;  ///< Stores all discovered shortest paths from start_path to end_path.
};
