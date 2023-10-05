#include "cache.hpp"
#include "lrucache.hpp"
#include "perfectcache.hpp"

#include <string>
#include <iostream>
#include <sstream>

using namespace caches;

std::vector<int> readInput() {
    std::string input;
    std::getline(std::cin, input);

    std::vector<int> requests;

    std::istringstream iss(input);
    int num;

    while (iss >> num) {
        requests.push_back(num);
    }

    if (!iss.eof() || (requests.size() < 2) || (requests[0] < 0) || (requests[1] != requests.size() - 2)) {
        std::cout << "Invalid input format\n";
        exit(1);
    }

    return requests;
}

int main() {
    int hits = 0;
    int n;
    size_t m;

    std::cin >> m >> n;
    if (!std::cin.good()) {
        std::cerr << "failed to read input\n";
        return 1;
    }

    caches::lru_2_cache lru2(m);

    for (int i = 0; i < n; i++) {
        int q;
        std::cin >> q;
        if (!std::cin.good()) {
            std::cerr << "failed to read input\n";
            return 1;
        }
        hits += lru2.lookup_update(q);
    }

    std::cout << hits << '\n';
}