#include "HttpClient.hpp"
#include "../include/CrawlerUtils.hpp"

#include <string>
#include <queue>
#include <unordered_set>
#include <vector>

class Crawler {
public:
    explicit Crawler(const std::string& base_url);

    void crawl(const std::string& start_path, int max_visit = 1000);

private:
    std::string base_url;
    std::queue<std::string> to_visit;
    std::unordered_set<std::string> visited;
    HttpClient http;

    std::vector<std::string> visit(const std::string& url);
};
