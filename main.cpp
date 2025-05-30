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
<<<<<<< HEAD
    ShortestPathGame processor(base_url, start_dir, end_dir);
    //processor.multi_find(1);
    processor.find();
=======


    std::string t = "https://en.wikipedia.org/wiki/France";
    Crawler processor(base_url, 100);
    std::cout << "init\n";
    processor.multi_crawl(t, 2, 14, 100);
>>>>>>> d67ea99781c3085ac44d55aeedcf8ca41b8e8a79
}