

#include "PageProcessing.h"




std::string PageProcessing::fetch_html(const std::string& url) {
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

bool PageProcessing::is_internal_link(const std::string& url) {
    // If the link is relative (starts with "/"), it's internal
    if (url.substr(0, 6) == "/wiki/" && url.substr(0, 14) != "/wiki/category") {  // Check if the URL starts with "/wiki/"
        return true;
    }    

    // // Check if the link is absolute and shares the same domain as the base URL
    // std::string domain = extract_domain(url);
    // if (!domain.empty() && domain == base_domain) {
    //     return true;
    // }

    return false;
}

std::vector<std::string> PageProcessing::extract_links(const std::string& html) {
    std::vector<std::string> links;
    std::regex href_pattern("<a\\s+[^>]*href=[\"']([^\"']+)[\"']", std::regex::icase);
    std::smatch match;

    std::string::const_iterator search_start(html.cbegin());
    while (std::regex_search(search_start, html.cend(), match, href_pattern)) {
        std::string link = match[1];
        
        // Filter out internal links based on the base domain
        if (is_internal_link(link)) {
            links.push_back(link);
        }
        
        search_start = match.suffix().first;
    }

    return links;
}



std::vector<std::string> PageProcessing::visit(const std::string& url) {
    std::string html_file = fetch_html(url);
    std::vector<std::string> links = extract_links(html_file);
    return links;
};