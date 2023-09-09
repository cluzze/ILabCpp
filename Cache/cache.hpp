#pragma once

#include <vector>
#include <list>
#include <unordered_map>

namespace caches {
    template<typename KeyT = int, typename Hash = std::hash<KeyT>>
    class Cache {
    public:
        Cache(int capacity);
    private:
        int cap;
        int Kin, Kout;
    };
}