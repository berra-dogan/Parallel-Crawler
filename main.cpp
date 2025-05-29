#include "include/Crawler.hpp"

int main(){
    std::string visit_dir = "/wiki/France";
    std::string base_url = "https://en.wikipedia.org";
    Crawler processor(base_url);
    processor.crawl(visit_dir);
}