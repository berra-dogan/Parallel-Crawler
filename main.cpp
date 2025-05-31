#include "include/Crawler.hpp"
#include "include/ShortestPathGame.hpp"

int main(){
    // std::string visit_dir = "/wiki/France";
    // std::string base_url = "https://en.wikipedia.org";
    // Crawler processor(base_url, 100);
    // processor.multi_crawl(visit_dir, 16);

    std::string start_dir = "/wiki/France";
    std::string end_dir = "/wiki/Coat_of_arms_of_France";
    std::string base_url = "https://en.wikipedia.org";


    std::string t = "https://en.wikipedia.org/wiki/France";
    Crawler processor(base_url, 1000, 100);
    std::cout << "init\n";

    auto start = std::chrono::steady_clock::now();
    processor.multi_crawl(t, 4, 14, 100);
    auto end = std::chrono::steady_clock::now();
    auto rt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Time taken: " << rt << std::endl;
    
}