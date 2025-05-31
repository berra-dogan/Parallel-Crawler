#include <fstream>
#include <iostream>
#include "include/Crawler.hpp"
#include "include/Crawler2.hpp"

int main(){
    std::string base_url = "https://en.wikipedia.org";
    std::string t = "https://en.wikipedia.org/wiki/France";

    // Crawler processor(base_url, 50);
    // processor.multi_crawl(t, 16);

    Crawler2 processor2(base_url, 50);
    processor2.multi_crawl(t, 2, 14);  // (start URL, depth, threads, max pages)

    // // Export graph
    // std::ofstream out("web_graph.dot");
    // out << "digraph Web {\n";

    // const auto& graph = processor.graph;
    // for (const auto& [from, links] : graph) {
    //     for (const auto& to : links) {
    //         out << "    \"" << from << "\" -> \"" << to << "\";\n";
    //     }
    // }

    // out << "}\n";
    // std::cout << "Graph written to web_graph.dot\n";
}
