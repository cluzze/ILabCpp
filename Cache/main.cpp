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