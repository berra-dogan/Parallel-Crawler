#include "../../include/Crawler.hpp"
#include "../../include/Crawler2.hpp"
#include "../../include/ShortestPathGame.hpp"
#include "../../include/CrawlerUtils.hpp"

int main(){
    std::string base_url = "https://en.wikipedia.org";
    std::string t = "https://en.wikipedia.org/wiki/France";
    std::string start_path = "/wiki/France";

    

    // int processor_n[] = {1, 4, 8, 16, 20};

    // for (auto i: processor_n) {
    //     std::cout << i << std::endl;
    //     Crawler2 processor(base_url, 100);

    //     
    //     processor.multi_crawl(base_url + start_path, 1, i);
    //     auto end = std::chrono::steady_clock::now();
    //     auto rt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    //     std::cout << "Processors: " << i << ", Running time : " << rt << std::endl;
    // }

    Crawler2 processor(base_url, 10000);

    auto start = std::chrono::steady_clock::now();
    processor.multi_crawl(start_path, 1, 15);
    auto end = std::chrono::steady_clock::now();
    auto rt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    //std::cout << "Processors: " << 3 << ", Running time : " << rt << std::endl;
        
    Page* root = processor.visited.get_obj(start_path);


    std::cout << root->neighbours.size() << std::endl;
    CrawlerUtils::graphing(root);
}
