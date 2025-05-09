#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <gumbo.h>

// Struct to store webpage contents
struct Memory {
    std::string data;
};

// libcurl callback to write data to Memory
size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    Memory* mem = static_cast<Memory*>(userp);
    mem->data.append(static_cast<char*>(contents), total_size);
    return total_size;
}

// Recursively search DOM for <a href="...">
void search_links(GumboNode* node, std::vector<std::string>& links) {
    if (node->type != GUMBO_NODE_ELEMENT) return;

    if (node->v.element.tag == GUMBO_TAG_A) {
        GumboAttribute* href = gumbo_get_attribute(&node->v.element.attributes, "href");
        if (href && href->value) {
            links.emplace_back(href->value);
        }
    }

    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        search_links(static_cast<GumboNode*>(children->data[i]), links);
    }
}

int main() {
    CURL* curl = curl_easy_init();
    Memory chunk;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://en.wikipedia.org/wiki/France");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
            curl_easy_cleanup(curl);
            return 1;
        }

        curl_easy_cleanup(curl);

        // Parse HTML with Gumbo
        GumboOutput* output = gumbo_parse(chunk.data.c_str());
        std::vector<std::string> links;
        search_links(output->root, links);
        gumbo_destroy_output(&kGumboDefaultOptions, output);

        // Print found links
        for (const auto& link : links) {
            std::cout << "Link: " << link << "\n";
        }
    } else {
        std::cerr << "Failed to initialize CURL\n";
        return 1;
    }

    return 0;
}
