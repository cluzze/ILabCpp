#include "tree.hpp"

#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <random>
#include <iostream>
#include <fstream>

using namespace containers;

std::vector<std::pair<char, int>> parseInput() {

    std::string input;
    std::vector<std::pair<char, int>> requests;

    std::getline(std::cin, input);

    std::regex pattern(R"(([kmn]\s-?\d+)\s*)");
    std::sregex_iterator iterator(input.begin(), input.end(), pattern);
    std::sregex_iterator end;

    while (iterator != end) {
        std::smatch match = *iterator;
        std::string matchStr = match.str();

        char type = matchStr[0];
        int value = std::stoi(matchStr.substr(2));

        requests.push_back({type, value});

        ++iterator;
    }
    
    return requests;
}

int main() {
    SearchTree<int> tree;
    using iterator = SearchTree<int>::iterator;

    std::vector<std::pair<char, int>> requests = parseInput();
    for (int i = 0; i < requests.size(); i++) {
        char type = requests[i].first;
        int value = requests[i].second;
        SearchTree<int>::iterator it;
        int res;
        switch (type) {
        case 'k':
            tree.insert(value);
            break;
        case 'm':
            it = tree.nth_element(value);
            if (it != tree.end())
                std::cout << it->key << '\n';
            break;
        case 'n':
            res = tree.cnt(value);
            std::cout << res << '\n';
            break;
        default:
            break;
        }
    }
}