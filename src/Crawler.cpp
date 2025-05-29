#include "../include/Crawler.hpp"

Crawler::Crawler(const std::string& base_url) : base_url(base_url) {}

std::vector<std::string> Crawler::visit(const std::string& url) {
    std::string html = http.fetch(url);
    std::string base_domain = CrawlerUtils::extract_domain(url);
    return CrawlerUtils::extract_links(html, base_domain);
}

void Crawler::crawl(const std::string& start_path, int max_visit) {
    int counter = 0;
    Page* starting = new Page(start_path, 0);
    to_visit.push(starting);
    visited.add(starting);

    while (!to_visit.is_empty() && counter < max_visit) {
        Page* visited_page = to_visit.pop();

        std::string current_url = base_url + visited_page->url;
        std::cout << current_url << std::endl;

        for (const auto& link : visit(current_url)) {
            if (CrawlerUtils::is_valid_link(link, base_url)) {
                Page* neighbour_ptr = visited.get_obj(link);
                if (!neighbour_ptr){
                    neighbour_ptr = new Page(link, visited_page->distance_from_initial_page+1);
                    to_visit.push(neighbour_ptr);
                }
                visited_page->neighbours.push_back(neighbour_ptr);
            }
        }

        ++counter;
    }

    std::cout << "Total visited: " << counter << std::endl;
}
