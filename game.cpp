#include "include/ShortestPathGame.hpp"

int main(){
    std::string start_dir = "/wiki/France";
    std::string end_dir = "/wiki/Bolivia" ; //"/wiki/Mesozoic";
    std::string base_url = "https://en.wikipedia.org";
    ShortestPathGame processor(base_url, start_dir, end_dir);
    processor.multi_find(32);
}