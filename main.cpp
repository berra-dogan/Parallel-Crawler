#include <iostream>
#include <string>
#include <vector>


#include "PageProcessing/PageProcessing.h"



int main() {
    PageProcessing processor;
    std::vector<std::string> links = processor.visit("https://en.wikipedia.org/wiki/Main_Page");


    //for (int i = 0; i < links.size(); i++) {
    //    std::cout << links[i] << std::endl;
    //}
    

}
