#pragma once

#include <string>
#include <vector>
#include "Crawler2.hpp"
#include "SafeUnboundedQueue.hpp"
#include "RefinableHashSet.hpp"
#include "HttpClient.hpp"

class ShortestPathGame : public Crawler2 {
    public:
        explicit ShortestPathGame(const std::string& base_url,
                                 const std::string& start_path,
                                 const std::string& end_path)
            : Crawler2(base_url, 0), start_path(start_path), end_path(end_path) {
                if (start_path == end_path) {
                    throw std::invalid_argument("Start and end are the same");
                }
            }
        
        void multi_find(size_t num_threads);
        void find(std::atomic<size_t>& active_threads);
    
    private:
        std::string start_path;            // Store as value
        std::string end_path;              // Store as value

        void find_path_solutions();
    
        std::atomic<size_t> best_depth{std::numeric_limits<size_t>::max()};
    
        // Store solutions as vectors of strings (paths)
        std::vector<std::vector<std::string>> solutions;
    };
    
