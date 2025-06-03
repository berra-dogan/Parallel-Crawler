#include "../include/CrawlerUtils.hpp"
#include <fstream>
#include <unordered_set>

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

    void graphing(std::unordered_map<std::string, std::unordered_set<std::string>>& graph){
        // Step 1: Compute in-degree for each node
        std::unordered_map<std::string, int> in_degree;
        for (const auto& [from, tos] : graph) {
            for (const auto& to : tos) {
                in_degree[to]++;
            }
            // Ensure all nodes appear in the map, even if in-degree is zero
            if (in_degree.find(from) == in_degree.end()) {
                in_degree[from] = 0;
            }
        }
    
        // Step 2: Open DOT file for writing
        std::ofstream out("web_graph.dot");
        out << "digraph Web {\n";
        out << "    node [shape=circle style=filled fillcolor=lightblue fixedsize=true fontname=\"Arial\"];\n";
    
        // Find the maximum in-degree for normalization
        int max_deg = 0;
        for (const auto& [node, deg] : in_degree) {
            max_deg = std::max(max_deg, deg);
        }
    
        // Step 3: Write nodes with size proportional to in-degree
        for (const auto& [node, deg] : in_degree) {
            double size = 1 + 4 * (double(deg) / std::max(1, max_deg));  // size in inches
            out << "    \"" << node << "\" [width=" << size << " height=" << size << "];\n";
        }
    
        // Step 4: Write edges
        for (const auto& [from, tos] : graph) {
            for (const auto& to : tos) {
                out << "    \"" << from << "\" -> \"" << to << "\";\n";
            }
        }
    
        out << "}\n";
    }
    

}