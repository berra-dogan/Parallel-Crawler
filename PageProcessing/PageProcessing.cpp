

#include "PageProcessing.h"

std::vector<std::string> PageProcessing::extract_links(const std::string& html) {
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