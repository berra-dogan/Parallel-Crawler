#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <cstdio>
#include <memory>



class PageProcessing {

    PageProcessing(){};


    std::string fetch_html(const std::string& url);

    std::vector<std::string> PageProcessing::extract_links(const std::string& html);
};