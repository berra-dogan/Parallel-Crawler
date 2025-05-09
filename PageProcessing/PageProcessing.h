#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <cstdio>
#include <memory>



class PageProcessing {
    public:

        PageProcessing(){};

        std::string fetch_html(const std::string& url);

        std::vector<std::string> extract_links(const std::string& html);

        std::vector<std::string> visit(const std::string& url);
};