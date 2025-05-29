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
    visited.insert(start_path);

    while (!to_visit.is_empty() && counter < max_visit) {
        Page* visited_page = to_visit.pop();

        std::string current_url = base_url + visited_page->url;
        std::cout << current_url << std::endl;

        for (const auto& link : visit(current_url)) {
            if (CrawlerUtils::is_valid_link(link, base_url)) {

                Page a(link, 1);
                if (set.contains(a)) {
                    
                } else {

                }





                if (visited.insert(link).second){
                    // PageInfo new_page 
                    to_visit.push(link);
                } else {
                    visited_page->neighbours.push_back()
                }
            }
        }

        ++counter;
    }

    std::cout << "Total visited: " << counter << std::endl;
}
