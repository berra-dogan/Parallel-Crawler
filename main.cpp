#include "include/Crawler.hpp"

int main(){
    std::string base_url = "https://en.wikipedia.org";
    std::string t = "https://en.wikipedia.org/wiki/France";
    Crawler processor(base_url, 100);
    std::cout << "init\n";
    processor.multi_crawl(t, 2, 14, 100);
}