#include <string>
#include <vector>
#include <regex>

namespace CrawlerUtils {

    std::string extract_domain(const std::string& url);

    bool is_valid_link(const std::string& url, const std::string& base_domain);

    std::vector<std::string> extract_links(const std::string& html, const std::string& base_domain);

}
