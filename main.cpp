#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Callback to handle data received from the server
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    fwrite(contents, 1, total_size, stdout); // Print to stdout
    return total_size;
}

int main(void) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);  // Initialize global libcurl state

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");           // Set target URL
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);       // Set callback for response data

        res = curl_easy_perform(curl);  // Perform the request

        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);  // Cleanup
    }

    curl_global_cleanup();  // Global cleanup
    return 0;
}
