#pragma once

#include <string>
#include <vector>

class Page {
    public:
        Page(std::string url, int distance_from_initial_page): url(url), distance_from_initial_page(distance_from_initial_page), neighbours({}) {}

        const std::string url;
        std::vector<Page*> neighbours;
        int distance_from_initial_page;


        bool operator==(const Page& other) const {
            return url == other.url;
        }
};