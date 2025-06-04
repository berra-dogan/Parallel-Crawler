#include "../include/Crawler.hpp"
#include <curl/curl.h>

Crawler1::Crawler1(const std::string& base_url, size_t max_visit, int batch_fetch_size) : base_url(base_url), batch_fetch_size(batch_fetch_size), max_visit(max_visit) {
    num_visited = 0;
}

Crawler1::Crawler1(const std::string& base_url, size_t max_visit) : base_url(base_url), batch_fetch_size(10), max_visit(max_visit){
    num_visited = 0;
}



std::vector<std::string> Crawler1::visit(const std::string& url) {
    std::string html = http.fetch(url);
    std::string base_domain = CrawlerUtils::extract_domain(url);
    return CrawlerUtils::extract_links(html, base_domain);
}


void Crawler1::find_depths(const std::string& start_path) {
    Page* start = visited.get_obj(start_path);
    if (!start) return;

    std::queue<Page*> q;
    start->depth = 0;
    q.push(start);

    while (!q.empty()) {
        Page* current = q.front();
        q.pop();

        for (Page* neighbor : current->neighbours) {
            int proposed_depth = current->depth + 1;
            if (neighbor->depth == -1 || neighbor->depth > proposed_depth) {
                neighbor->depth = proposed_depth;
                q.push(neighbor);
            }
        }
    }
}

void Crawler1::multi_crawl(const std::string& start_path, size_t num_threads){
    Page* starting = new Page(start_path);
    starting->depth = 0;
    to_visit.push(starting);
    visited.add(starting);

    std::vector<std::thread> threads(num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
        threads[i] = std::thread(&Crawler1::crawl, this, start_path);
    }

    for (auto& th : threads) {
        th.join();
    }

    find_depths(start_path);

    std::cout << "Total visited: " << num_visited << std::endl;
}

void Crawler1::crawl(const std::string& start_path) {
    while (num_visited.load() < max_visit) {

        Page* visited_page = to_visit.pop();

        // Atomically increment after popping to avoid going beyond max_visit
        if (num_visited.fetch_add(1) >= max_visit) {
            break;
        }

        std::string current_url = base_url + visited_page->url;
        std::cout << current_url << std::endl;

        for (const auto& link : visit(current_url)) {
            if (CrawlerUtils::is_valid_link(link, base_url)) {

                Page* neighbour_ptr = visited.get_obj(link);
                if (!neighbour_ptr) {
                    neighbour_ptr = new Page(link);
                    visited.add(neighbour_ptr);
                    to_visit.push(neighbour_ptr);
                } 
                visited_page->neighbours.insert(neighbour_ptr);

            }
        }
    }
}