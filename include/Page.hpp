
#include <string>
#include <vector>



class Page {
    public:
        Page(std::string url, int distance_from_initial_page): url(url), distance_from_initial_page(distance_from_initial_page) {}

        const std::string url;
        const std::vector<Page*> neighbours = {};
        int distance_from_initial_page;


        bool operator==(const Page& other) const {
            return url == other.url;
        }
};


// structs for hashing a PageInfo and comparing.
// Those are used in the RefinableHashSet class
struct PageInfoPtrHash {
    std::size_t operator()(const Page* p) const {
        return std::hash<std::string>{}(p->url);
    }
};

struct PageInfoPtrEqual {
    bool operator()(const Page* a, const Page* b) const {
        return a->url == b->url;
    }
};