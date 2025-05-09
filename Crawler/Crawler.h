#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <cstdio>
#include <memory>



class Crawler {
    public:

        Crawler(){};

        std::string fetch_html(const std::string& url);

        std::vector<std::string> extract_links(const std::string& html, const std::string& base_domain);

        std::string extract_domain(const std::string& url);
        bool is_internal_link(const std::string& url, const std::string& base_domain);

        std::vector<std::string> visit(const std::string& url);
};