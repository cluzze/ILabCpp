#pragma once

#include <vector>
#include <list>
#include <unordered_map>

namespace caches {
    template<typename KeyT = int, typename Hash = std::hash<KeyT>>
    class LruCache {
    public:
        LruCache(int capacity) : cap{capacity} {}

        bool full() { return cache_.size() == cap; }

        bool lookup_update(KeyT key); 
    private:
        int cap;
        std::list<KeyT> cache_;
        using ListIt = std::list<KeyT>::iterator;
        std::unordered_map<KeyT, ListIt> hash_;
    };
    template <typename KeyT, typename Hash>
    bool LruCache<KeyT, Hash>::lookup_update(KeyT key)
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