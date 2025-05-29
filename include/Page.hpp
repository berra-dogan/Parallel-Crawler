#pragma once

#include <string>
#include <vector>

class Page {
    public:
        Page(std::string url, int depth): url(url), depth(depth), neighbours({}) {}

        const std::string url;
        int depth;
        std::vector<Page*> neighbours;

        bool operator==(const Page& other) const {
            return url == other.url;
        }
};