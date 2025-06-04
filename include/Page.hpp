#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include <atomic>


/**
 * @class Page
 * @brief Represents a web page in the crawling graph.
 * 
 * This class holds the URL of the page, its depth in a traversal,
 * connections to neighboring pages (via hyperlinks), and optional
 * previous pages for reverse path computations.
 */
class Page {
public:
    /**
     * @brief Constructs a Page with a given URL and default depth -1.
     * 
     * @param url The URL of the page.
     */
    Page(std::string url)
        : url(std::move(url)), depth(-1), neighbours({}), prev_pages({}) {}

    /**
     * @brief Constructs a Page with a given URL and specified depth.
     * 
     * @param url The URL of the page.
     * @param depth The depth level in the traversal or search.
     */
    Page(std::string url, int depth)
        : url(std::move(url)), depth(depth), neighbours({}), prev_pages({}) {}

    /// The unique URL identifying this web page.
    const std::string url;

    /// The depth of the page in a graph traversal. Default is -1 (unvisited).
    std::atomic<int> depth;

    /// Set of neighboring pages reachable via links from this page.
    std::unordered_set<Page*> neighbours;

    /**
     * @brief Set of pages that link to this page.
     * 
     * This is only used in the context of the ShortestPathGame 
     * or reverse traversal computations.
     */
    std::unordered_set<Page*> prev_pages;

    /// The raw HTML content of the page.
    std::string page_content;

    /**
     * @brief Equality operator based on page URL.
     * 
     * @param other The other Page to compare with.
     * @return True if URLs are equal; false otherwise.
     */
    bool operator==(const Page& other) const {
        return url == other.url;
    }
};