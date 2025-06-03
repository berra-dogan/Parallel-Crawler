#include "../../include/Crawler.hpp"
#include "../../include/Crawler2.hpp"
#include "../../include/ShortestPathGame.hpp"
#include "../../include/CrawlerUtils.hpp"

int main(){
    int n_threads= 16;

    std::string base_url = "https://en.wikipedia.org";
    std::string start_path = "/wiki/France";

    Crawler1 processor(base_url, 10000);

    auto start = std::chrono::steady_clock::now();
    processor.multi_crawl(start_path, n_threads);
    auto end = std::chrono::steady_clock::now();
    auto rt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Threads: " << n_threads << ", Running time : " << rt << std::endl;
        
    CrawlerUtils::graphing(processor.graph);

}
