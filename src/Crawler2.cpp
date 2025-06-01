#include "../include/Crawler2.hpp"
#include <curl/curl.h>

Crawler2::Crawler2(const std::string& base_url, size_t max_visit) : base_url(base_url), max_visit(max_visit) {
    num_visited = 0;
}



/*
void Crawler2::link_fetcher() {
    while (num_visited < max_visit) {
        Page* current = to_fetch.pop();


        current->page_content = http.fetch(current->url);
        to_process.push(current);
    }
}*/
/*
void Crawler2::link_fetcher() {
    CURLM* multi = curl_multi_init();

    //std::cout << max_visit << std::endl;
    while (num_visited.load() < max_visit) {
        //std::cout << "link_fetcher\n" << std::endl;
        std::vector<Page*> batch;
        Page* current;

        for (int i = 0; i < batch_fetch_size; i++ ) {
            current = to_fetch.pop_no_busy_waiting();
            if (current == NULL) {
                break;
            } else {
                batch.push_back(current);
            }
        }
        //std::cout << "link_fetcher\n" << std::endl;
        

        //std::cout << batch.size() << std::endl;
        // CURL to Page in order to know which request is for which link
        std::unordered_map<CURL*, Page*> handle_to_page;

        for (auto& page : batch) {
            //std::cout << page << std::endl;
            // in case a page has been added to to_fetch even though it's already visited
            // Shouldn't be the case tho

            //std::cout << "URL: " << page->url << std::endl;

            CURL* easy = curl_easy_init();
            page->page_content.clear();  // make sure it's empty

            curl_easy_setopt(easy, CURLOPT_URL, page->url.c_str());
            curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, +[](char* ptr, size_t size, size_t nmemb, void* userdata) -> size_t {
                auto* content = static_cast<std::string*>(userdata);
                content->append(ptr, size * nmemb);
                return size * nmemb;
            });
            curl_easy_setopt(easy, CURLOPT_WRITEDATA, &page->page_content);
            curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, 1L);
            curl_multi_add_handle(multi, easy);

            handle_to_page[easy] = page;
        }

        int running_handles;
        curl_multi_perform(multi, &running_handles);


        // wait for all requests to be sent then handle the responses
        while (running_handles > 0) {
            curl_multi_wait(multi, nullptr, 0, 1000, nullptr);
            curl_multi_perform(multi, &running_handles);


            
            CURLMsg* msg;
            int msgs_left;
            while ((msg = curl_multi_info_read(multi, &msgs_left))) {
                if (msg->msg == CURLMSG_DONE) {
                    CURL* easy = msg->easy_handle;
                    auto page = handle_to_page[easy];

                    curl_multi_remove_handle(multi, easy);
                    curl_easy_cleanup(easy);

                    to_process.push(page);
                }
            }
        }
    }

    curl_multi_cleanup(multi);
}*/


/*
void Crawler2::link_fetcher() {
    CURLM* multi = curl_multi_init();

    constexpr int MAX_ACTIVE = 8;

    std::unordered_map<CURL*, Page*> handle_to_page;
    std::set<CURL*> active_handles;

    // std::cout << max_visit << std::endl;
    while (num_visited.load() < max_visit) {
        // std::cout << "link_fetcher\n" << std::endl;

        // Fill up the active handle set up to MAX_ACTIVE
        //std::cout << active_handles.size() << std::endl;
        while (active_handles.size() < MAX_ACTIVE) {
            std::vector<Page*> batch;
            Page* current;

            int remaining_slots = MAX_ACTIVE - active_handles.size();
            for (int i = 0; i < remaining_slots && i < batch_fetch_size; ++i) {

                current = to_fetch.pop_no_busy_waiting();
                if (current == NULL) {
                    break;
                } else {
                    batch.push_back(current);
                }
            }

            // std::cout << "link_fetcher\n" << std::endl;

            for (auto& page : batch) {
                // std::cout << page << std::endl;

                // std::cout << "URL: " << page->url << std::endl;

                CURL* easy = curl_easy_init();
                page->page_content.clear();  // make sure it's empty

                curl_easy_setopt(easy, CURLOPT_URL, page->url.c_str());
                curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, +[](char* ptr, size_t size, size_t nmemb, void* userdata) -> size_t {
                    auto* content = static_cast<std::string*>(userdata);
                    content->append(ptr, size * nmemb);
                    return size * nmemb;
                });
                curl_easy_setopt(easy, CURLOPT_WRITEDATA, &page->page_content);
                curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, 1L);
                curl_easy_setopt(easy, CURLOPT_TIMEOUT, 10L); // optional: timeout after 10s

                CURLMcode mcode = curl_multi_add_handle(multi, easy);
                if (mcode != CURLM_OK) {
                    std::cerr << "curl_multi_add_handle failed: " << curl_multi_strerror(mcode) << std::endl;
                    curl_easy_cleanup(easy);
                    continue;
                }

                handle_to_page[easy] = page;
                active_handles.insert(easy);
            }

            if (batch.empty()) {
                break; // no more work right now
            }
        }

        // wait for any of the active requests to complete
        int still_running = 0;
        curl_multi_wait(multi, nullptr, 0, 1000, nullptr);
        curl_multi_perform(multi, &still_running);

        // handle responses as they complete
        CURLMsg* msg;
        int msgs_left;
        while ((msg = curl_multi_info_read(multi, &msgs_left))) {
            if (msg->msg == CURLMSG_DONE) {
                CURL* easy = msg->easy_handle;

                auto it = handle_to_page.find(easy);
                if (it != handle_to_page.end()) {
                    Page* page = it->second;

                    // check result of the transfer
                    if (msg->data.result != CURLE_OK) {
                        std::cerr << "Error fetching " << page->url
                                  << ": " << curl_easy_strerror(msg->data.result) << std::endl;
                        delete page; // or recycle if you want
                    } else {
                        to_process.push(page);
                    }

                    // Cleanup
                    curl_multi_remove_handle(multi, easy);
                    curl_easy_cleanup(easy);
                    handle_to_page.erase(it);
                    active_handles.erase(easy);
                }
            }
        }
    }

    curl_multi_cleanup(multi);
}
*/



void Crawler2::link_fetcher() {
    CURLM* multi = curl_multi_init();
    constexpr int MAX_ACTIVE = 8;
    std::unordered_map<CURL*, Page*> handle_to_page;
    std::set<CURL*> active_handles;

    while (num_visited.load() < max_visit) {
        // Fill up active handles
        while (active_handles.size() < MAX_ACTIVE) {
            std::vector<Page*> batch;
            Page* current;

            int remaining_slots = MAX_ACTIVE - active_handles.size();
            for (int i = 0; i < remaining_slots && i < batch_fetch_size; ++i) {
                current = to_fetch.pop_no_busy_waiting();
                if (current == nullptr) break;
                batch.push_back(current);
            }

            for (auto& page : batch) {
                CURL* easy = curl_easy_init();
                page->page_content.clear();

                curl_easy_setopt(easy, CURLOPT_URL, page->url.c_str());
                curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, +[](char* ptr, size_t size, size_t nmemb, void* userdata) -> size_t {
                    auto* content = static_cast<std::string*>(userdata);
                    content->append(ptr, size * nmemb);
                    return size * nmemb;
                });
                curl_easy_setopt(easy, CURLOPT_WRITEDATA, &page->page_content);
                curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, 1L);
                curl_easy_setopt(easy, CURLOPT_TIMEOUT, 10L);
                curl_easy_setopt(easy, CURLOPT_CONNECTTIMEOUT, 5L);
                curl_easy_setopt(easy, CURLOPT_USERAGENT, "Mozilla/5.0 (compatible; MiniCrawler/1.0)");

                CURLMcode mcode = curl_multi_add_handle(multi, easy);
                if (mcode != CURLM_OK) {
                    std::cerr << "curl_multi_add_handle failed: " << curl_multi_strerror(mcode) << std::endl;
                    curl_easy_cleanup(easy);
                    continue;
                }

                handle_to_page[easy] = page;
                active_handles.insert(easy);
            }

            if (batch.empty()) break; // no more work
        }

        // Wait and perform
        int still_running = 0, numfds = 0;
        CURLMcode wait_code = curl_multi_wait(multi, nullptr, 0, 1000, &numfds);
        if (wait_code != CURLM_OK) {
            std::cerr << "curl_multi_wait failed: " << curl_multi_strerror(wait_code) << std::endl;
        }

        CURLMcode mc;
        do {
            mc = curl_multi_perform(multi, &still_running);
        } while (mc == CURLM_CALL_MULTI_PERFORM);

        // Process completed transfers
        CURLMsg* msg;
        int msgs_left;
        while ((msg = curl_multi_info_read(multi, &msgs_left))) {
            if (msg->msg == CURLMSG_DONE) {
                CURL* easy = msg->easy_handle;

                auto it = handle_to_page.find(easy);
                if (it != handle_to_page.end()) {
                    Page* page = it->second;

                    if (msg->data.result != CURLE_OK) {
                        std::cerr << "Error fetching " << page->url
                                  << ": " << curl_easy_strerror(msg->data.result) << std::endl;

                        // Retry once
                        to_fetch.push(page);
                    } else {
                        to_process.push(page);
                    }

                    curl_multi_remove_handle(multi, easy);
                    curl_easy_cleanup(easy);
                    handle_to_page.erase(it);
                    active_handles.erase(easy);
                }
            }
        }

        // Avoid tight loop if nothing to do
        if (active_handles.empty() && to_fetch.is_empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

    curl_multi_cleanup(multi);
}



void Crawler2::link_processor() {
    while (num_visited < max_visit) {
        //std::cout << "hey\n";
        std::cout << to_process.is_empty() << std::endl;
        Page* page = to_process.pop();

        // Extract links from the html
        std::string base_domain = CrawlerUtils::extract_domain(page->url);
        std::vector<std::string> links = CrawlerUtils::extract_links(page->page_content, base_domain);

        num_visited.fetch_add(1);
        std::cout << num_visited <<std::endl;

        std::cout << page->url << std::endl;

        // we don't need to store all the html after having proceessed the link so we remove it 
        // this saves memory and is needed with the amount of pages visited
        page->page_content.clear(); 

        for (std::string& link : links) {
            graph[page->url].insert(link);

            std::string l = base_url + link;

            if (visited.contains(l)) {
                continue;
            } 
            // add any new links to page and to to_fetch
            // Like this only one link should ever be added to to_fetch I think
            Page* new_page = new Page(l);
            visited.add(new_page);
            to_fetch.push(new_page);

        }
    }
}



std::vector<std::string> Crawler2::visit(const std::string& url) {
    std::string html = http.fetch(url);
    std::string base_domain = CrawlerUtils::extract_domain(url);
    return CrawlerUtils::extract_links(html, base_domain);
}


void Crawler2::multi_crawl(const std::string& start_path, size_t num_threads_fetch, size_t num_threads_process){
    Page* starting = new Page(start_path);
    starting->depth = 0;
    to_fetch.push(starting);
    visited.add(starting);


    std::vector<std::thread> threads_fetch(num_threads_fetch);
    std::vector<std::thread> threads_process(num_threads_process);

    for (size_t i = 0; i < num_threads_fetch; ++i) {
        threads_fetch[i] = std::thread(&Crawler2::link_fetcher, this);
    }

    for (size_t i = 0; i < num_threads_process; ++i) {
        threads_process[i] = std::thread(&Crawler2::link_processor, this);
    }

    for (auto& th : threads_fetch) {
        th.join();
    }
    
    for (auto& th : threads_process) {
        th.join();
    }
}