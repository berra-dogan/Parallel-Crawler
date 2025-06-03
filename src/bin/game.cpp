#include "../../include/ShortestPathGame.hpp"

int main(){
    std::string base_url = "https://en.wikipedia.org";
    std::string t = "https://en.wikipedia.org/wiki/France";
    std::string start_path = "/wiki/France";
    std::string end_path = "/wiki/Spider";

    ShortestPathGame game(base_url, start_path, end_path);
    game.multi_find(10);
}