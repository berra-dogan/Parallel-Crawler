#include "../include/HttpClient.hpp"

std::string HttpClient::fetch(const std::string& url) {
    std::string command = "curl -s \"" + url + "\"";
    std::string result;
    char buffer[128];

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Failed to run curl\n";
        return "";
    }

    while (fgets(buffer, sizeof(buffer), pipe)) {
        result += buffer;
    }

    pclose(pipe);
    return result;
}