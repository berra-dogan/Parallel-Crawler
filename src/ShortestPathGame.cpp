#include "../include/ShortestPathGame.hpp"
#include <cassert>

void ShortestPathGame::multi_find(size_t num_threads){

    Page* starting = new Page(start_path);
    starting->depth = 0;
    to_visit.push(starting);
    visited.add(starting);

    std::vector<std::thread> threads(num_threads);
    std::atomic<size_t> active_threads{num_threads};

    for (size_t i = 0; i < num_threads; ++i) {
        threads[i] = std::thread(&ShortestPathGame::find, this, std::ref(active_threads));
    }

    for (auto& th : threads) {
        th.join();
    }

    ShortestPathGame::find_path_solutions();
    
}

void ShortestPathGame::find_path_solutions() {

    if (best_depth == std::numeric_limits<size_t>::max()) {
        std::cout << "No path found." << std::endl;
        return;
    }

    Page* end_page = visited.get_obj(end_path);
    if (!end_page) {
        std::cout << "End page not found in visited." << std::endl;
        return;
    }

    std::vector<std::vector<std::string>> all_paths;
    std::vector<std::string> current_path;

    std::function<void(Page*)> dfs = [&](Page* node) {
        current_path.push_back(node->url);
        if (node->depth == 0) {
            std::vector<std::string> path_copy = current_path;
            std::reverse(path_copy.begin(), path_copy.end());
            all_paths.push_back(path_copy);
        } else {
            for (Page* prev : node->prev_pages) {
                if (prev->depth == node->depth - 1) {
                    dfs(prev);
                }
            }
        }
        current_path.pop_back();
    };

    dfs(end_page);

    solutions = all_paths;

    std::cout << "Found " << all_paths.size() << " shortest path(s):" << std::endl;
    for (const auto& path : all_paths) {
        for (const auto& url : path) {
            std::cout << url << " -> ";
        }
        std::cout << "[END]" << std::endl;
    }
}


void ShortestPathGame::find(std::atomic<size_t>& active_threads) {
    while (true) {
        Page* visited_page = to_visit.pop_no_busy_waiting();

        if (!visited_page) {
            active_threads.fetch_sub(1);
            if (active_threads.load(std::memory_order_acquire) == 0) {
                to_visit.not_empty.notify_all();
                std::cout << "Best depth: " << best_depth << std::endl;
                return;
            }

            std::unique_lock<std::mutex> lk(to_visit.lock);
            while (to_visit.elements.empty()) {
                if (active_threads.load(std::memory_order_acquire) == 0) return;
                to_visit.not_empty.wait(lk);
            }

            active_threads.fetch_add(1);
            continue;
        }

        std::string current_url = base_url + visited_page->url;
        std::cout << current_url << "     " << best_depth << std::endl;

        if (visited_page->depth >= int(best_depth)) continue;

        for (const auto& link : visit(current_url)) {
            if (!CrawlerUtils::is_valid_link(link, base_url)) continue;

            Page* neighbour_ptr = visited.get_obj(link);
            size_t candidate_depth = visited_page->depth + 1;

            if (!neighbour_ptr) {
                neighbour_ptr = new Page(link, candidate_depth);
                visited.add(neighbour_ptr);
                to_visit.push(neighbour_ptr);
            } else {
                int old_depth = neighbour_ptr->depth;
                if (candidate_depth < old_depth) {
                    neighbour_ptr->depth = candidate_depth;
                    to_visit.push(neighbour_ptr);
                }
            }

            neighbour_ptr->prev_pages.insert(visited_page);

            if (link == end_path) {
                size_t prev_best = best_depth.load();
                while (candidate_depth < prev_best &&
                       !best_depth.compare_exchange_weak(prev_best, candidate_depth)) {}
            }
        }
    }
}
