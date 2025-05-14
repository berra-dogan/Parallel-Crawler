#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <set>

#include "include/Crawler.hpp"
#include "include/PageInfo.hpp"


int main() {
    std::string base_url = "https://en.wikipedia.org";
    
    std::string visit_dir = "/wiki/France";
    Crawler processor(base_url);
    processor.crawl(visit_dir);

}
