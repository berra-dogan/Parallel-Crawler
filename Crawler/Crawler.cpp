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

    while (fgets(buffer, sizeof(buffer), pipe)) {
        result += buffer;
    }

    pclose(pipe);
    return result;
}

bool Crawler::is_valid_link(const std::string& url, const std::string& base_domain) {
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

std::string Crawler::extract_domain(const std::string& url) {
    std::regex domain_pattern(R"(https?://([a-zA-Z0-9.-]+))");
    std::smatch match;
    if (std::regex_search(url, match, domain_pattern)) {
        return match[1];
    }
    return "";
}

std::vector<std::string> Crawler::extract_links(const std::string& html, const std::string& base_domain) {
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

std::vector<std::string> Crawler::visit(const std::string& url) {
    std::string html = fetch_html(url);
    std::string domain = extract_domain(url);
    return extract_links(html, domain);
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
            if (is_valid_link(link, base_url) && visited.insert(link).second) {
                to_visit.push(link);
            }
        }

        ++counter;
    }

    std::cout << "Total visited: " << counter << std::endl;
}
