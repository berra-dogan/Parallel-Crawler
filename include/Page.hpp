#pragma once

#include <string>
#include <vector>
#include <unordered_set>

class Page {
    public:
        Page(std::string url, int depth): url(url), depth(depth), neighbours({}) {}

        const std::string url;
        int depth;
        std::unordered_set<Page*> neighbours;
        std::unordered_set<Page*> prev_pages;


        std::string page_content;


        bool operator==(const Page& other) const {
            return url == other.url;
        }
};