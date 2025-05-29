
#include <string>
#include <vector>



class PageInfo {
    public:
        PageInfo(std::string url, std::vector<std::string> neighbours, int distance_from_initial_page): url(url), neighbours(neighbours), distance_from_initial_page(distance_from_initial_page) {}

        const std::string url;
        const std::vector<std::string> neighbours;
        int distance_from_initial_page;

        bool operator==(const PageInfo& other) const {
            return url == other.url;
        }
};




// structs for hashing a PageInfo and comparing.
// Those are used in the RefinableHashSet class
struct PageInfoPtrHash {
    std::size_t operator()(const PageInfo* p) const {
        return std::hash<std::string>{}(p->url);
    }
};

struct PageInfoPtrEqual {
    bool operator()(const PageInfo* a, const PageInfo* b) const {
        return a->url == b->url;
    }
};