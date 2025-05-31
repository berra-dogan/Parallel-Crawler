#include "../include/Crawler.hpp"
#include <curl/curl.h>

Crawler::Crawler(const std::string& base_url, size_t max_visit, int batch_fetch_size) : base_url(base_url), batch_fetch_size(batch_fetch_size), max_visit(max_visit) {
    num_visited = 0;
}

Crawler::Crawler(const std::string& base_url, size_t max_visit) : base_url(base_url), batch_fetch_size(10), max_visit(max_visit){
    num_visited = 0;
}



std::vector<std::string> Crawler::visit(const std::string& url) {
    std::string html = http.fetch(url);
    std::string base_domain = CrawlerUtils::extract_domain(url);
    return CrawlerUtils::extract_links(html, base_domain);
}


void Crawler::multi_crawl(const std::string& start_path, size_t num_threads){
    Page* starting = new Page(start_path, 0);
    to_visit.push(starting);
    visited.add(starting);

    std::vector<std::thread> threads(num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
        threads[i] = std::thread(&Crawler::crawl, this, start_path);
    }

    for (auto& th : threads) {
        th.join();
    }

    std::cout << "Total visited: " << num_visited << std::endl;
}

void Crawler::crawl(const std::string& start_path) {
    while (num_visited.load() < max_visit) {

        Page* visited_page = to_visit.pop();

        for (Page* prev: visited_page->prev_pages){
            graph[prev->url].insert(visited_page->url);
        }

        // Atomically increment after popping to avoid going beyond max_visit
        if (num_visited.fetch_add(1) >= max_visit) {
            break;
        }

        std::string current_url = base_url + visited_page->url;
        //std::cout << current_url << std::endl;

        for (const auto& link : visit(current_url)) {
            if (CrawlerUtils::is_valid_link(link, base_url)) {

                Page* neighbour_ptr = visited.get_obj(link);
                if (!neighbour_ptr) {
                    neighbour_ptr = new Page(link, visited_page->depth + 1);
                    visited.add(neighbour_ptr);
                    to_visit.push(neighbour_ptr);
                } else if (visited_page->depth + 1 < neighbour_ptr->depth) {
                    neighbour_ptr->depth = visited_page->depth + 1;
                    to_visit.push(neighbour_ptr);
                }
                neighbour_ptr->prev_pages.insert(visited_page);

            }
        }
    }
}
