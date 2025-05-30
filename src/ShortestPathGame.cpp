#include "../include/ShortestPathGame.hpp"
#include <cassert>

void ShortestPathGame::multi_find(size_t num_threads){

    Page* starting = new Page(start_path, 0);
    to_fetch.push(starting);
    visited.add(starting);

    std::vector<std::thread> threads(num_threads);
    std::atomic<size_t> active_threads{num_threads};

    for (size_t i = 0; i < num_threads; ++i) {
        threads[i] = std::thread(&ShortestPathGame::find, this, std::ref(active_threads));
    }

    for (auto& th : threads) {
        th.join();
    }
}

void ShortestPathGame::find(std::atomic<size_t>& active_threads){

    while (true) {
        Page* visited_page = to_fetch.pop_no_busy_waiting();
        
        if (visited_page==nullptr){
            active_threads.fetch_sub(1);
            if (active_threads==0){
                std::cout << "Best depth: " << best_depth << std::endl;
                to_fetch.not_empty.notify_all();
                return;
            };
            std::unique_lock<std::mutex> lk(to_fetch.lock);
            while (to_fetch.elements.empty()) {
                if (active_threads==0) return;
                to_fetch.not_empty.wait(lk);
            }
            active_threads.fetch_add(1);  
            continue;
        }  

        if (best_depth != std::numeric_limits<size_t>::max() && visited_page->depth >= best_depth) {
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
    }
}




// #include "include/Crawler.hpp"

// int main(){
//     std::string start_dir = "/wiki/France";
//     std::string end_dir = "/wiki/France";
//     std::string base_url = "https://en.wikipedia.org";
//     Crawler processor(base_url, 1000);
//     processor.multi_crawl(visit_dir, 16);
// }