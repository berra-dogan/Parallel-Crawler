#include "../../include/ShortestPathGame.hpp"
#include <iostream>
#include <chrono>

int main() {
    std::string base_url = "https://en.wikipedia.org";
    std::string start_path = "/wiki/École_polytechnique";
    std::string end_path = "/wiki/Spider";

    ShortestPathGame game(base_url, start_path, end_path, true);

    auto start_time = std::chrono::high_resolution_clock::now();
    
    game.multi_find(8);
    
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    std::cout << "Execution time: " << duration.count() << " seconds\n";

    return 0;
}


// earlier depths should be prioritized