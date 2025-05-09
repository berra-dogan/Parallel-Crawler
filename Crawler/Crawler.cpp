#include "Crawler.h"

std::string Crawler::fetch_html(const std::string& url) {
    std::string command = "curl -s \"" + url + "\"";
    std::string result;
    char buffer[128];

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Failed to run curl\n";
        return "";
    }

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);
    return result;
}


bool Crawler::is_internal_link(const std::string& url, const std::string& base_domain) {
    // Check if the URL contains a colon
    if (url.find(":") != std::string::npos || 
        url.find("?") != std::string::npos || 
        url.find("#") != std::string::npos || 
        url.find("&") != std::string::npos || 
        url.find("@") != std::string::npos) {
        return false;  // Return false if the URL contains any of these characters
    }

    if (url.substr(0, 1) == "/" && url.substr(0, 2) != "//") {
        return true;
    }

    // Check if the link is absolute and shares the same domain as the base URL
    std::string domain = extract_domain(url);
    if (!domain.empty() && domain == base_domain) {
        return true;
    }

    return false;
}

std::string Crawler::extract_domain(const std::string& url) {
    // A regex to extract domain from URL
    std::regex domain_pattern(R"(https?://([a-zA-Z0-9.-]+))");
    std::smatch match;
    if (std::regex_search(url, match, domain_pattern)) {
        return match[1].str();  // Return the domain
    }
    return "";  // Return empty string if domain extraction fails
}

std::vector<std::string> Crawler::extract_links(const std::string& html, const std::string& base_domain) {
    std::vector<std::string> links;
    // Regex pattern to extract href links from HTML
    std::regex href_pattern("<a\\s+[^>]*href=[\"']([^\"']+)[\"']", std::regex::icase);
    std::smatch match;

    std::string::const_iterator search_start(html.cbegin());

    // Extract all the links from the HTML content
    while (std::regex_search(search_start, html.cend(), match, href_pattern)) {
        std::string link = match[1];

        // Filter out internal links based on the base domain
        if (is_internal_link(link, base_domain)) {
            links.push_back(link);
        }

        search_start = match.suffix().first;
    }

    return links;
}

std::vector<std::string> Crawler::visit(const std::string& url) {
    std::string html_file = fetch_html(url);

    // Extract the base domain from the URL
    std::string base_domain = extract_domain(url);
    // Extract internal links
    std::vector<std::string> links = extract_links(html_file, base_domain);
    return links;
};


void Crawler::crawl(const std::string& visit_dir, int max_visit){
    std::vector<std::string> links;

    int counter_visited = 0;
    to_visit.push(visit_dir);
    visited.emplace(visit_dir);


    while (to_visit.size() > 0) {
        std::string current_link = base_url + to_visit.front();

        std::cout << current_link << std::endl;
        to_visit.pop();
        links = visit(current_link);
    
        std::vector<std::string> valid_links;
        for (int i = 0; i < links.size(); i++) {
            if (is_internal_link(links[i], base_url) && !(visited.find(links[i]) != visited.end())) {
                to_visit.push(links[i]);
                visited.emplace(links[i]);
            }
        }
        
        counter_visited++;
        if (counter_visited > max_visit){
            break;
        }
    }
    
    std::cout << counter_visited << std::endl;
}
