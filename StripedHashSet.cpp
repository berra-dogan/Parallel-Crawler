#include <iostream>
#include <unordered_set>
#include <mutex>
#include <vector>
#include <thread>

template <typename T>
class StripedHashSet {
private:
    std::vector<std::unordered_set<T>> stripes;
    std::vector<std::mutex> locks;

    size_t num_stripes;

    size_t get_stripe_index(const T& key) {
        return std::hash<T>()(key) % num_stripes;
    }

public:
    StripedHashSet(size_t num_stripes) : num_stripes(num_stripes) {
        stripes.resize(num_stripes);
        locks.resize(num_stripes);
    }

    void insert(const T& key) {
        size_t index = get_stripe_index(key);
        std::lock_guard<std::mutex> lock(locks[index]);
        stripes[index].insert(key);
    }

    bool contains(const T& key) {
        size_t index = get_stripe_index(key);
        std::lock_guard<std::mutex> lock(locks[index]);
        return stripes[index].count(key) > 0;
    }

    void remove(const T& key) {
        size_t index = get_stripe_index(key);
        std::lock_guard<std::mutex> lock(locks[index]);
        stripes[index].erase(key);
    }
};

int main() {
    StripedHashSet<std::string> set(10); // 10 stripes
    set.insert("page1");
    set.insert("page2");
    set.insert("page3");

    std::cout << set.contains("page1") << std::endl; // Should print 1 (true)
    std::cout << set.contains("page4") << std::endl; // Should print 0 (false)

    return 0;
}
