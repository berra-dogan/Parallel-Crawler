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
    ShortestPathGame processor(base_url, start_dir, end_dir);
    //processor.multi_find(1);
    processor.find();
}