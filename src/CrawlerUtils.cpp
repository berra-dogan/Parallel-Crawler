#include "../include/CrawlerUtils.hpp"

namespace CrawlerUtils {

    bool is_valid_link(const std::string& url, const std::string& base_domain) {
        // Ignore URLs with unwanted characters
        if (url.find_first_of(":#?&@") != std::string::npos)
            return false;
    
        // Relative link
        if (url.substr(0, 1) == "/" && url.substr(0, 2) != "//") {
            return true;
        }
    
        // Absolute link with matching domain
        return extract_domain(url) == base_domain;
    }
    
    std::string extract_domain(const std::string& url) {
        std::regex domain_pattern(R"(https?://([a-zA-Z0-9.-]+))");
        std::smatch match;
        if (std::regex_search(url, match, domain_pattern)) {
            return match[1];
        }
        return "";
    }
    
    std::vector<std::string> extract_links(const std::string& html, const std::string& base_domain) {
        std::vector<std::string> links;
        std::regex href_pattern(R"(<a\s+[^>]*href=["']([^"']+)["'])", std::regex::icase);
        auto begin = html.cbegin(), end = html.cend();
        std::smatch match;
    
        while (std::regex_search(begin, end, match, href_pattern)) {
            const std::string& link = match[1];
            if (is_valid_link(link, base_domain)) {
                links.push_back(link);
            }
            begin = match.suffix().first;
        }
    
        return links;
    }
    

}