#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <string>
#include <unordered_set>
#include <list>
#include <iomanip> 
#include "../../include/Page.hpp"
#include "../../include/RefinableHashSet.hpp"

class CoarseHashSet {
    private:
        std::unordered_set<std::string> set;
        std::mutex mtx;
    
    public:
        bool add(Page* page) {
            std::lock_guard<std::mutex> lock(mtx);
            return set.insert(page->url).second;
        }
    
        bool remove(const std::string& url) {
            std::lock_guard<std::mutex> lock(mtx);
            return set.erase(url) > 0;
        }
    
        bool contains(const std::string& url) {
            std::lock_guard<std::mutex> lock(mtx);
            return set.count(url) > 0;
        }
    
        Page* get_obj(const std::string& url) {
            std::lock_guard<std::mutex> lock(mtx);
            return set.count(url) > 0 ? new Page(url) : nullptr;
        }
    };

class FineGrainedHashSet {
    private:
        static constexpr int NUM_BUCKETS = 256;
        std::vector<std::list<Page*>> table;
        std::vector<std::mutex> locks;
    
    public:
        FineGrainedHashSet()
            : table(NUM_BUCKETS), locks(NUM_BUCKETS) {}
    
        bool add(Page* x) {
            size_t idx = std::hash<std::string>{}(x->url) % NUM_BUCKETS;
            std::lock_guard<std::mutex> lock(locks[idx]);
            for (auto* page : table[idx]) {
                if (page->url == x->url) return false;
            }
            table[idx].push_back(x);
            return true;
        }
    
        bool remove(const std::string& url) {
            size_t idx = std::hash<std::string>{}(url) % NUM_BUCKETS;
            std::lock_guard<std::mutex> lock(locks[idx]);
            auto& bucket = table[idx];
            auto it = std::find_if(bucket.begin(), bucket.end(),
                                    [&](Page* p) { return p->url == url; });
            if (it != bucket.end()) {
                bucket.erase(it);
                return true;
            }
            return false;
        }
    
        bool contains(const std::string& url) {
            size_t idx = std::hash<std::string>{}(url) % NUM_BUCKETS;
            std::lock_guard<std::mutex> lock(locks[idx]);
            for (auto* page : table[idx]) {
                if (page->url == url) return true;
            }
            return false;
        }
    
        Page* get_obj(const std::string& url) {
            size_t idx = std::hash<std::string>{}(url) % NUM_BUCKETS;
            std::lock_guard<std::mutex> lock(locks[idx]);
            for (auto* page : table[idx]) {
                if (page->url == url) return page;
            }
            return nullptr;
        }
    };
    
    
template <typename SetType>
void benchmark_set(const std::string& name, SetType& set, int num_threads, int ops_per_thread) {

    auto worker = [&](int tid) {
        std::mt19937 gen(tid);  // Thread-local RNG
        std::uniform_int_distribution<> dist(0, ops_per_thread);

        for (int i = 0; i < ops_per_thread; ++i) {
            std::string url = "http://example.com/page" + std::to_string(dist(gen));
            int op = dist(gen) % 3;
            if (op == 0) {
                Page* p = new Page{url};
                set.add(p);
            } else if (op == 1) {
                set.remove(url);
            } else {
                set.contains(url);
            }
        }
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker, i);
    }

    for (auto& t : threads) t.join();

}

void print_table_header() {
    std::cout << "+-----------+------------------------+------------------------+------------------------+------------------------------+------------------------------+\n";
    std::cout << "| Threads   | Refinable Time (s)     | FineGrained Time (s)   | Coarse Time (s)        | Speedup (Refinable/Coarse)   | Speedup (Refinable/Fine)     |\n";
    std::cout << "+-----------+------------------------+------------------------+------------------------+------------------------------+------------------------------+\n";
}

void print_table_row(int threads, double r, double f, double c) {
    std::cout << "| "
              << std::setw(9) << threads << " | "
              << std::setw(22) << std::fixed << std::setprecision(3) << r << " | "
              << std::setw(22) << f << " | "
              << std::setw(22) << c << " | "
              << std::setw(28) << c / r << " | "
              << std::setw(28) << f / r << " |\n";
}

void print_table_footer() {
    std::cout << "+-----------+------------------------+------------------------+------------------------+------------------------------+------------------------------+\n";
}

int main() {
    const std::vector<int> thread_counts = {1, 2, 3, 4, 5, 6, 8, 16, 20};
    const int ops_per_thread = 100000;

    print_table_header();

    for (int num_threads : thread_counts) {
        RefinableHashSet refinable_set;
        FineGrainedHashSet fine_grained_set;
        CoarseHashSet coarse_set;

        auto start_r = std::chrono::high_resolution_clock::now();
        benchmark_set("RefinableHashSet", refinable_set, num_threads, ops_per_thread);
        auto duration_r = std::chrono::duration<double>(
            std::chrono::high_resolution_clock::now() - start_r).count();

        auto start_f = std::chrono::high_resolution_clock::now();
        benchmark_set("FineGrainedHashSet", fine_grained_set, num_threads, ops_per_thread);
        auto duration_f = std::chrono::duration<double>(
            std::chrono::high_resolution_clock::now() - start_f).count();

        auto start_c = std::chrono::high_resolution_clock::now();
        benchmark_set("CoarseHashSet", coarse_set, num_threads, ops_per_thread);
        auto duration_c = std::chrono::duration<double>(
            std::chrono::high_resolution_clock::now() - start_c).count();

        print_table_row(num_threads, duration_r, duration_f, duration_c);
    }

    print_table_footer();
    return 0;
}
