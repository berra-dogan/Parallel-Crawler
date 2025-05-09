#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <cstdio>
#include <memory>
#include <queue>
#include <set>


class Crawler {
    public:

        Crawler(std::string base_url = "https://en.wikipedia.org") : base_url(base_url){};

        std::string fetch_html(const std::string& url);

        std::vector<std::string> extract_links(const std::string& html, const std::string& base_domain);

        std::string extract_domain(const std::string& url);
        bool is_internal_link(const std::string& url, const std::string& base_domain);

        std::vector<std::string> visit(const std::string& url);
        void crawl(const std::string& visit_dir, int max_visit = 1000);

        std::string base_url;
        std::queue<std::string> to_visit;
        std::set<std::string> visited;
        
};