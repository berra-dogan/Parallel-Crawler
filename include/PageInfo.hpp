
#include <string>
#include <vector>



class PageInfo {
    public:
        PageInfo(std::string url, std::vector<std::string> neighbours, int distance_from_initial_page): url(url), neighbours(neighbours), distance_from_initial_page(distance_from_initial_page) {}

        const std::string url;
        const std::vector<std::string> neighbours;
        int distance_from_initial_page;
};