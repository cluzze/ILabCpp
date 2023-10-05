#pragma once

#include <vector>
#include <list>
#include <unordered_map>

namespace caches {
    template<typename KeyT = int, typename Hash = std::hash<KeyT>>
    class lru_cache {
    public:
        using size_type = size_t;
    public:
        lru_cache(size_type capacity) : cap{capacity} {}

        void remove(KeyT key);

        bool full() const { return cache_.size() == cap; }

        bool isPresent(KeyT key) const { return hash_.find(key) != hash_.end(); }

        bool lookup_update(KeyT key);
    private:
        size_type cap;
        std::list<KeyT> cache_;
        using ListIt = std::list<KeyT>::iterator;
        std::unordered_map<KeyT, ListIt, Hash> hash_;
    };

    template <typename KeyT, typename Hash>
    void lru_cache<KeyT, Hash>::remove(KeyT key) {
        auto hit = hash_.find(key);
        if (hit == hash_.end()) {
            auto cacheIt = hit->second;
            cache_.erase(cacheIt);
            hash_.erase(key);
        }
    }

    template <typename KeyT, typename Hash>
    bool lru_cache<KeyT, Hash>::lookup_update(KeyT key)
    {
        auto hit = hash_.find(key);
        if (hit == hash_.end()) {
            if (full()) {
                hash_.erase(cache_.back());
                cache_.pop_back();
            }
            cache_.push_front(key);
            hash_[key] = cache_.begin();
            return false;
        }

        auto cacheIt = hit->second;
        if (cacheIt != cache_.begin())
            cache_.splice(cache_.begin(), cache_, cacheIt, std::next(cacheIt));
        return true;
    }
}