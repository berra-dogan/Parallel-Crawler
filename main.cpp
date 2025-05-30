#include "include/Crawler.hpp"
#include "include/ShortestPathGame.hpp"

int main(){
    // std::string base_url = "https://en.wikipedia.org";
    // std::string t = "https://en.wikipedia.org/wiki/France";
    // Crawler processor(base_url, 100);
    // std::cout << "init\n";
    // processor.multi_crawl(t, 2, 14, 100);

    std::string start_dir = "/wiki/France";
    std::string end_dir = "/wiki/Coat_of_arms_of_France";
    std::string base_url = "https://en.wikipedia.org";
    ShortestPathGame processor(base_url, start_dir, end_dir);
    processor.multi_find(4);
    //processor.find();
}