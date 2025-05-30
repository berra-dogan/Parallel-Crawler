#include "../include/Crawler.hpp"
#include <curl/curl.h>

Crawler::Crawler(const std::string& base_url, size_t max_visit) : base_url(base_url), max_visit(max_visit) {
    num_visited = 0;
}



void Crawler::link_fetcher() {
    CURLM* multi = curl_multi_init();

    while (true) {
        //std::cout << "link_fetcher\n" << std::endl;
        std::vector<Page*> batch;
        Page* current;

        for (int i = 0; i < batch_fetch_size; i++ ) {
            current = to_fetch.pop_no_busy_waiting();
            std::cout << current << std::endl;
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
            std::cout << page << std::endl;
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
}


void Crawler::link_processor() {
    while (true) {
        //std::cout << "hey\n";
        Page* page = to_process.pop();

        // Extract links from the html
        std::string base_domain = CrawlerUtils::extract_domain(page->url);
        std::vector<std::string> links = CrawlerUtils::extract_links(page->page_content, base_domain);


        std::cout << page->url << std::endl;

        // we don't need to store all the html after having proceessed the link so we remove it 
        // this saves memory and is needed with the amount of pages visited
        page->page_content.clear(); 

        for (std::string& link : links) {
            std::string l = base_url + link;

            if (visited.contains(l)) {
                continue;
            } 
            // add any new links to page and to to_fetch
            // Like this only one link should ever be added to to_fetch I think
            Page* new_page = new Page(l, page->depth + 1);
            visited.add(new_page);
            to_fetch.push(new_page);
        }
    }
}



std::vector<std::string> Crawler::visit(const std::string& url) {
    std::string html = http.fetch(url);
    std::string base_domain = CrawlerUtils::extract_domain(url);
    return CrawlerUtils::extract_links(html, base_domain);
}


void Crawler::multi_crawl(const std::string& start_path, size_t num_threads_fetch, size_t num_threads_process, int max_visit){
    Page* starting = new Page(start_path, 0);
    to_fetch.push(starting);
    visited.add(starting);


    std::vector<std::thread> threads_fetch(num_threads_fetch);
    std::vector<std::thread> threads_process(num_threads_process);

    for (size_t i = 0; i < num_threads_fetch; ++i) {
        threads_fetch[i] = std::thread(&Crawler::link_fetcher, this);
    }

    for (size_t i = 0; i < num_threads_process; ++i) {
        threads_process[i] = std::thread(&Crawler::link_processor, this);
    }

    for (auto& th : threads_fetch) {
        th.join();
    }
    
    for (auto& th : threads_process) {
        th.join();
    }
}


/*
void Crawler::multi_crawl(const std::string& start_path, size_t num_threads, int max_visit){
    Page* starting = new Page(start_path, 0);
    to_visit.push(starting);
    visited.add(starting);

    std::vector<std::thread> threads(num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
        threads[i] = std::thread(&Crawler::crawl, this, start_path);
    }

    for (auto& th : threads) {
        th.join();
    }
}*/

/*
void Crawler::crawl(const std::string& start_path, int max_visit) {
    int counter = 0;

    while (num_visited.load() < max_visit) {
        Page* visited_page = to_visit.pop();

        std::string current_url = base_url + visited_page->url;
        std::cout << current_url << std::endl;

        for (const auto& link : visit(current_url)) {
            if (CrawlerUtils::is_valid_link(link, base_url)) {
                Page* neighbour_ptr = visited.get_obj(link);
                if (!neighbour_ptr){
                    neighbour_ptr = new Page(link, visited_page->depth+1);
                    to_visit.push(neighbour_ptr);
                } else {
                    neighbour_ptr->depth = std::min(neighbour_ptr->depth, visited_page->depth+1);
                }
                visited_page->neighbours.push_back(neighbour_ptr);
            }
        }

        num_visited.fetch_add(1);
    }

    std::cout << "Total visited: " << counter << std::endl;
}*/
