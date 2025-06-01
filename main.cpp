#include <fstream>
#include <iostream>
#include <cmath>
#include "include/Crawler.hpp"
#include "include/Crawler2.hpp"
#include "include/ShortestPathGame.hpp"

int main(){
    std::string base_url = "https://en.wikipedia.org";
    std::string t = "https://en.wikipedia.org/wiki/France";
    std::string start_path = "/wiki/France";
    std::string end_path = "/wiki/Spider";

    ShortestPathGame game(base_url, start_path, end_path);
    game.multi_find(16);
}

int _main(){
    std::string base_url = "https://en.wikipedia.org";
    std::string t = "https://en.wikipedia.org/wiki/France";
    std::string start_path = "/wiki/France";

    

    // int processor_n[] = {1, 4, 8, 16, 20};

    // for (auto i: processor_n) {
    //     std::cout << i << std::endl;
    //     Crawler2 processor(base_url, 100);

    //     
    //     processor.multi_crawl(base_url + start_path, 1, i);
    //     auto end = std::chrono::steady_clock::now();
    //     auto rt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    //     std::cout << "Processors: " << i << ", Running time : " << rt << std::endl;
    // }

    Crawler2 processor(base_url, 10000);

    auto start = std::chrono::steady_clock::now();
    processor.multi_crawl(base_url + start_path, 1, 16);
    auto end = std::chrono::steady_clock::now();
    auto rt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Processors: " << 3 << ", Running time : " << rt << std::endl;



    // Crawler2 processor2(base_url, 50);
    // processor2.multi_crawl(t, 2, 14);  // (start URL, depth, threads, max pages)

    // Export graph

    //std::cout << processor.visited.get_obj("/wiki/Francophone_economy")->depth;

    // ... rest of your graph
        

    std::unordered_map<std::string, std::unordered_set<std::string>> graph = processor.graph;

    // Step 1: Compute in-degree
    std::unordered_map<std::string, int> in_degree;
    for (const auto& [from, tos] : graph) {
        for (const auto& to : tos) {
            in_degree[to]++;
        }
        if (in_degree.find(from) == in_degree.end()) {
            in_degree[from] = 0;  // Ensure all nodes are included
        }
    }

    // Step 2: Write DOT file
    std::ofstream out("web_graph.dot");
    out << "digraph Web {\n";
    out << "    node [shape=circle style=filled fillcolor=lightblue fixedsize=true fontname=\"Arial\"];\n";

    // Optionally normalize sizes
    int max_deg = 0;
    for (const auto& [node, deg] : in_degree) {
        max_deg = std::max(max_deg, deg);
    }

    for (const auto& [node, deg] : in_degree) {
        double size = 1 + 4 * (double(deg) / std::max(1, max_deg));  // Size in inches
        out << "    \"" << node << "\" [width=" << size << " height=" << size << "];\n";
    }

    // Step 3: Write edges
    for (const auto& [from, links] : graph) {
        for (const auto& to : links) {
            out << "    \"" << from << "\" -> \"" << to << "\";\n";
        }
    }

    out << "}\n";

}
