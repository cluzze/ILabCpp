#pragma once

#include <vector>
#include <list>
#include <unordered_map>
#include <cstddef>

#include "lrucache.hpp"

namespace caches {
    template<typename KeyT = int, typename Hash = std::hash<KeyT>>
    class lru_2_cache {
    public:
        using size_type = size_t;
    public:
        lru_2_cache(size_type capacity) : cap{capacity}, candidatePages{cap / 2}, hotPages{cap - (cap / 2)} {}

        bool full() const { return candidatePages.full() && hotPages.full(); }

        bool lookup_update(KeyT key);

        bool isPresent(KeyT key) const {
            return hotPages.isPresent(key) || candidatePages.isPresent(key);
        }

    private:
        bool tryFindFreeSlots(KeyT key);

    private:
        size_type cap;
        lru_cache<KeyT, Hash> candidatePages;
        lru_cache<KeyT, Hash> hotPages;
    };

    template <typename KeyT, typename Hash>
    bool lru_2_cache<KeyT, Hash>::lookup_update(KeyT key) {
        if (hotPages.isPresent(key)) {
            hotPages.lookup_update(key);
            return true;
        }

        if (candidatePages.isPresent(key)) {
            candidatePages.remove(key);
            hotPages.lookup_update(key);
            return true;
        }
        
        if (tryFindFreeSlots(key))
            return false;

        if (candidatePages.full()) {
            candidatePages.lookup_update(key);
            return false;
        }

        hotPages.lookup_update(key);

        return false;
    }

    template <typename KeyT, typename Hash>
    bool lru_2_cache<KeyT, Hash>::tryFindFreeSlots(KeyT key) {
        if (full())
            return false;

        if (!candidatePages.full()) {
            candidatePages.lookup_update(key);
            return true;
        }

        hotPages.lookup_update(key);
        return true;
    }
}