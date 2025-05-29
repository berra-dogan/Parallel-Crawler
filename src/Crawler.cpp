#include "../include/Crawler.hpp"

Crawler::Crawler(const std::string& base_url) : base_url(base_url) {}

std::vector<std::string> Crawler::visit(const std::string& url) {
    std::string html = http.fetch(url);
    std::string base_domain = CrawlerUtils::extract_domain(url);
    return CrawlerUtils::extract_links(html, base_domain);
}

void Crawler::crawl(const std::string& start_path, int max_visit) {
    int counter = 0;
    to_visit.push(start_path);
    visited.insert(start_path);

    while (!to_visit.empty() && counter < max_visit) {
        std::string relative_path = to_visit.front();
        to_visit.pop();

        std::string current_url = base_url + relative_path;
        std::cout << current_url << std::endl;

        for (const auto& link : visit(current_url)) {
            if (CrawlerUtils::is_valid_link(link, base_url) && visited.insert(link).second) {
                to_visit.push(link);
            }
        }

        ++counter;
    }

    std::cout << "Total visited: " << counter << std::endl;
}
