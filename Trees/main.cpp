#include "tree.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <regex>

using namespace containers;

std::vector<std::pair<char, int>> parseInput() {
    std::regex pattern(R"(([kmn])(\d+))");
    std::smatch matches;
    std::string input;
    std::vector<std::pair<char, int>> requests;

    while (std::getline(std::cin, input)) {
        if (std::regex_match(input, matches, pattern)) {
            char type = matches[1].str()[0];
            int value = std::stoi(matches[2].str());
            
            if (type == 'm' && value < 1) {
                std::cerr << "Invalid input" << '\n';
            }
            
            requests.push_back({type, value});
        }
        else {
            std::cerr << "Invalid input" << '\n';
        }
    }
}

int main() {
    SearchTree<int> tree;

    std::vector<std::pair<char, int>> requests = parseInput();
    for (int i = 0; i < requests.size(); i++) {
        char type = requests[i].first;
        int value = requests[i].second;

        switch (type) {
        case 'k':
            tree.insert(value);
            break;
        case 'm':
            int res = tree.nth_element(value);
            std::cout << res << '\n';
            break;
        case 'n':
            int res = tree.cnt(value);
            std::cout << res << '\n';
            break;
        default:
            break;
        }
    }
}