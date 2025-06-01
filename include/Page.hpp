#pragma once

#include <string>
#include <vector>
#include <unordered_set>

class Page {
    public:
        Page(std::string url)
        : url(std::move(url)), depth(-1), neighbours({}), prev_pages({}) {}

        Page(std::string url, int depth)
            : url(std::move(url)), depth(depth), neighbours({}), prev_pages({}) {}

        const std::string url;
        std::atomic<int> depth;
        std::unordered_set<Page*> neighbours;
        std::unordered_set<Page*> prev_pages; //Only used for the ShortestPathGame

        std::string page_content;


        bool operator==(const Page& other) const {
            return url == other.url;
        }
};