#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <set>

#include "Crawler/Crawler.h"
#include "PageInfo/PageInfo.h"


int main() {
    Crawler processor;
    std::vector<std::string> links = processor.visit("https://en.wikipedia.org/wiki/France");


    //for (int i = 0; i < links.size(); i++) {
      //  std::cout << links[i] << std::endl;
    //}

    std::string base_url = "https://en.wikipedia.org";


    std::queue<std::string> to_visit;
    to_visit.push("/wiki/France");
    std::set<std::string> visited;

    int counter_visited = 0;

    while (to_visit.size() > 0) {
        std::string current_link = base_url + to_visit.front();

        std::cout << current_link << std::endl;
        to_visit.pop();
        links = processor.visit(current_link);


    
        std::vector<std::string> valid_links;
        for (int i = 0; i < links.size(); i++) {
            if (processor.is_internal_link(links[i], "wikipedia.org") && !(visited.find(links[i]) != visited.end())) {
                to_visit.push(links[i]);
                visited.emplace(links[i]);
            }
        }
        counter_visited++;
        if (counter_visited >1000){
            break;
        }
    }
    
    std::cout << counter_visited << std::endl;

}
