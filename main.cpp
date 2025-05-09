#include <iostream>
#include <string>
#include <vector>


#include "Crawler/Crawler.h"



int main() {
    Crawler processor;
    std::vector<std::string> links = processor.visit("https://en.wikipedia.org/wiki/France");


    for (int i = 0; i < links.size(); i++) {
        std::cout << links[i] << std::endl;
    }


}
