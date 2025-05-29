#include <string>
#include <vector>
#include <regex>

/**
 * @namespace CrawlerUtils
 * @brief Utility functions used by the Crawler for URL processing and HTML parsing.
 */
namespace CrawlerUtils {

    /**
     * @brief Extracts the domain from a given URL.
     * 
     * This function parses the input URL and returns the domain portion,
     * stripping out the protocol and path.
     * 
     * @param url The input URL (e.g., "https://example.com/page").
     * @return A string containing the domain (e.g., "example.com").
     */
    std::string extract_domain(const std::string& url);

    /**
     * @brief Checks if a URL is a valid and internal link.
     * 
     * Determines whether the given URL should be visited by the crawler,
     * based on its structure and whether it belongs to the base domain.
     * 
     * @param url The URL to validate.
     * @param base_domain The domain that the crawler is restricted to.
     * @return True if the link is internal or absolute link with matching domain; false otherwise.
     */
    bool is_valid_link(const std::string& url, const std::string& base_domain);

    /**
     * @brief Extracts all valid links from an HTML page.
     * 
     * Uses regular expressions to parse the HTML content and find anchor (`<a>`) tag hrefs.
     * Filters the results to include only links considered valid by `is_valid_link`.
     * 
     * @param html The raw HTML content of a webpage.
     * @param base_domain The domain used to filter internal links.
     * @return A vector of valid links found in the HTML content.
     */
    std::vector<std::string> extract_links(const std::string& html, const std::string& base_domain);

}
