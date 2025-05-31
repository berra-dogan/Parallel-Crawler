#include <fstream>
#include <iostream>
#include "include/Crawler.hpp"
#include "include/Crawler2.hpp"

int main(){
    std::string base_url = "https://en.wikipedia.org";
    std::string t = "https://en.wikipedia.org/wiki/France";
    std::string start_path = "/wiki/France";

    

    int processor_n[] = {1, 4, 8, 16, 20};

    for (auto i: processor_n) {
        std::cout << i << std::endl;
        Crawler2 processor(base_url, 100);

        auto start = std::chrono::steady_clock::now();
        processor.multi_crawl(base_url + start_path, 1, i);
        auto end = std::chrono::steady_clock::now();
        auto rt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Processors: " << i << ", Running time : " << rt << std::endl;
    }


    // Crawler2 processor2(base_url, 50);
    // processor2.multi_crawl(t, 2, 14);  // (start URL, depth, threads, max pages)

    // Export graph
    /*
    std::ofstream out("web_graph.dot");
    out << "digraph Web {\n";

    const auto& graph = processor.graph;
    for (const auto& [from, links] : graph) {
        for (const auto& to : links) {
            out << "    \"" << from << "\" -> \"" << to << "\";\n";
        }
    }

    out << "}\n";
    std::cout << "Graph written to web_graph.dot\n";*/
}
