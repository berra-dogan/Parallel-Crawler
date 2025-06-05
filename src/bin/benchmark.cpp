#include "../../include/Crawler.hpp"
#include "../../include/Crawler2.hpp"
#include "../../include/ShortestPathGame.hpp"
#include "../../include/CrawlerUtils.hpp"


int main() {
    std::string base_url = "https://en.wikipedia.org";
    std::string start_path = "/wiki/France";

    int processor_n[] = {1, 2, 3, 4, 8, 16, 20};
    int visit_n[] = {10, 100, 1000};

    std::ofstream out("benchmark_results.txt");
    if (!out) {
        std::cerr << "Failed to open benchmark_results.txt for writing." << std::endl;
        return 1;
    }

    const int max_retries = 1;

    out << "Crawler1 Benchmark Results\n";
    for (auto j : visit_n) {
        out << "Visit count: " << j << std::endl;
        for (auto i : processor_n) {
            bool success = false;
            int retries = 0;
            long long rt = -1;

            while (!success && retries <= max_retries) {
                try {
                    Crawler1 processor(base_url, j);
                    auto start = std::chrono::steady_clock::now();
                    processor.multi_crawl(base_url + start_path, i);
                    auto end = std::chrono::steady_clock::now();
                    rt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                    success = true;
                } catch (const std::exception& e) {
                    out << "[Crawler1 Retry " << retries << "] Processors: " << i
                        << ", Visit count: " << j << ", Error: " << e.what() << "\n";
                    retries++;
                } catch (...) {
                    out << "[Crawler1 Retry " << retries << "] Processors: " << i
                        << ", Visit count: " << j << ", Unknown error\n";
                    retries++;
                }
            }

            if (success) {
                out << "Processors: " << i << ", Running time: " << rt << " ms\n";
            } else {
                out << "Processors: " << i << ", Visit count: " << j << ", Final result: Failed after retries\n";
            }
        }
        out << "\n";
    }

    out << "Crawler2 Benchmark Results\n";
    for (auto j : visit_n) {
        out << "Visit count: " << j << std::endl;
        for (auto i : processor_n) {
            bool success = false;
            int retries = 0;
            long long rt = -1;

            while (!success && retries <= max_retries) {
                try {
                    Crawler2 processor(base_url, j);
                    auto start = std::chrono::steady_clock::now();
                    processor.multi_crawl(base_url + start_path, 1, i);
                    auto end = std::chrono::steady_clock::now();
                    rt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                    success = true;
                } catch (const std::exception& e) {
                    out << "[Crawler2 Retry " << retries << "] Processors: " << i
                        << ", Visit count: " << j << ", Error: " << e.what() << "\n";
                    retries++;
                } catch (...) {
                    out << "[Crawler2 Retry " << retries << "] Processors: " << i
                        << ", Visit count: " << j << ", Unknown error\n";
                    retries++;
                }
            }

            if (success) {
                out << "Processors: " << i << ", Running time: " << rt << " ms\n";
            } else {
                out << "Processors: " << i << ", Visit count: " << j << ", Final result: Failed after retries\n";
            }
        }
        out << "\n";
    }

    out.close();
    return 0;
}