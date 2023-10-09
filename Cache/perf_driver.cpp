#include "cache.hpp"
#include "lrucache.hpp"
#include "perfectcache.hpp"

#include <string>
#include <iostream>
#include <sstream>

using namespace caches;

int main() {
    int hits = 0;
    int n;
    size_t m;

    std::cin >> m >> n;
    if (!std::cin.good()) {
        std::cerr << "failed to read input\n";
        return 1;
    }
    
    std::vector<int> input(n);
    for (int i = 0; i < n; i++) {
        std::cin >> input[i];
        if (!std::cin.good()) {
            std::cerr << "failed to read input\n";
            return 1;
        }
    }

    caches::perfect_cache perf(m, input.begin(), input.end());
    for (int i = 0; i < n; i++) {
        hits += perf.lookup_update(input[i]);
    }
    std::cout << hits << '\n';
}