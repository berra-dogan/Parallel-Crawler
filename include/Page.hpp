
#include <string>
#include <vector>



class Page {
    public:
        Page(std::string url, int distance_from_initial_page): url(url), distance_from_initial_page(distance_from_initial_page) {}

        const std::string url;
        const std::vector<Page*> neighbours = {};
        int distance_from_initial_page;
};