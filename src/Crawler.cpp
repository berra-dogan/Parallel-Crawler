#include "../include/Crawler.hpp"

Crawler::Crawler(const std::string& base_url, size_t max_visit) : base_url(base_url), max_visit(max_visit) {
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
}

void Crawler::crawl(const std::string& start_path) {

    while (num_visited.load() < max_visit) {
        Page* visited_page = to_visit.pop();

        std::string current_url = base_url + visited_page->url;
        std::cout << current_url << std::endl;

        for (const auto& link : visit(current_url)) {
            if (CrawlerUtils::is_valid_link(link, base_url)) {
                Page* neighbour_ptr = visited.get_obj(link);
                if (!neighbour_ptr){
                    neighbour_ptr = new Page(link, visited_page->depth+1);
                    to_visit.push(neighbour_ptr);
                } else {
                    neighbour_ptr->depth = std::min(neighbour_ptr->depth, visited_page->depth+1);
                }
                visited_page->neighbours.push_back(neighbour_ptr);
            }
        }

        num_visited.fetch_add(1);
    }

    std::cout << "Total visited: " << num_visited << std::endl;
}
