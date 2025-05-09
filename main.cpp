#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <set>

#include "Crawler/Crawler.h"
#include "PageInfo/PageInfo.h"


int main() {
    std::string base_url = "https://en.wikipedia.org";
    
    std::string visit_dir = "/wiki/France";
    Crawler processor(base_url);
    processor.crawl(visit_dir);

}
