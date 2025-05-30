#include "../include/ShortestPathGame.hpp"
#include <cassert>

void ShortestPathGame::multi_find(size_t num_threads){
    Page* starting = new Page(start_path, 0);
    to_fetch.push(starting);
    visited.add(starting);

    std::vector<std::thread> threads(num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
        threads[i] = std::thread(&ShortestPathGame::find, this);
    }

    for (auto& th : threads) {
        th.join();
    }
}

void ShortestPathGame::find(std::atomic<size_t> active_threads){
    if (start_path == end_path) {
        throw std::invalid_argument("Start and end are the same");
    }

    size_t visited_count = 0;

    while (visited_count < 1000) {
        Page* visited_page = to_fetch.pop();

        if (best_depth != std::numeric_limits<size_t>::max() && visited_page->depth >= best_depth) {
            ++visited_count;
            continue;
        }

        std::string current_url = base_url + visited_page->url;
        std::cout << current_url << std::endl;

        for (const auto& link : visit(current_url)) {
            if (CrawlerUtils::is_valid_link(link, base_url)) {
                std::cout << link << "    " << best_depth << "\n";
                if (link == end_path) {
                    size_t candidate_depth = visited_page->depth + 1;
                    //assert(candidate_depth <= best_depth);
                    if (candidate_depth < best_depth) {
                        best_depth = candidate_depth;
                        // store best path if you track it
                    } else {
                        // add alternative solution
                    }
                }

                Page* neighbour_ptr = visited.get_obj(link);
                if (!neighbour_ptr) {
                    neighbour_ptr = new Page(link, visited_page->depth + 1);
                    to_fetch.push(neighbour_ptr);
                } else if (visited_page->depth + 1 < neighbour_ptr->depth) {
                    neighbour_ptr->depth = visited_page->depth + 1;
                    to_fetch.push(neighbour_ptr);
                }

                visited_page->neighbours.push_back(neighbour_ptr);
            }
        }

        visited_count++;
    }

    std::cout << "Best depth: " << best_depth << std::endl;
}




// #include "include/Crawler.hpp"

// int main(){
//     std::string start_dir = "/wiki/France";
//     std::string end_dir = "/wiki/France";
//     std::string base_url = "https://en.wikipedia.org";
//     Crawler processor(base_url, 1000);
//     processor.multi_crawl(visit_dir, 16);
// }