

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



std::vector<std::string> Crawler::extract_links(const std::string& html) {
    std::vector<std::string> links;
    std::regex href_pattern("<a\\s+[^>]*href=[\"']([^\"']+)[\"']", std::regex::icase);
    std::smatch match;

    std::string::const_iterator search_start(html.cbegin());
    while (std::regex_search(search_start, html.cend(), match, href_pattern)) {
        links.push_back(match[1]);
        search_start = match.suffix().first;
    }

    return links;
}



std::vector<std::string> Crawler::visit(const std::string& url) {
    std::string html_file = fetch_html(url);
    std::vector<std::string> links = extract_links(html_file);
    return links;
};