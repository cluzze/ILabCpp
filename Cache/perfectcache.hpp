#pragma once

#include <vector>
#include <list>
#include <unordered_map>
#include <deque>
#include <iostream>
#include <cstddef>

namespace caches {
    template <typename KeyT = int, typename Hash = std::hash<KeyT>>
    class perfect_cache {
    public:
        using size_type = size_t;
    
    public:
        template <typename It>
        perfect_cache(size_type size, It begin, It end) : size_{size} {
            size_type i = 0;
            while (begin != end) {
                auto find = mp_.find(*begin);
                if (find == mp_.end())
                    mp_.insert({*begin, std::deque<size_type>{i}});
                else
                    find->second.push_back(i);
                i++;
                begin++;
            }
        }

        bool full() const { return cache_.size() == size_; }

        bool lookup_update(const KeyT& key) {
            auto find = hash_.find(key);
            if (find == hash_.end()) {
                if (full())
                    erase();
                cache_.push_front(key);
                mp_[key].pop_front();
                hash_[key] = cache_.begin();
                return false;
            }

            mp_[key].pop_front();
            return true;
        }

        bool isPresent(KeyT key) const {
            return hash_.find(key) != hash_.end();
        }

        void dump() const {
            std::cout << "cache:";
            for (auto x : cache_) {
                std::cout << " " << x;
            }
            std::cout << "\nmap: ";
            for (auto x : mp_) {
                std::cout << x.first << ":";
                for (auto i : x.second) {
                    std::cout << " " << i; 
                }
                std::cout << '\n';
            }
            std::cout << "\n";
        }
    private:
        void erase() {
            ListIt elem;
            int max = 0;
            for (ListIt it = cache_.begin(); it != cache_.end(); ++it) {
                auto find = mp_.find(*it);
                if (find->second.empty()) {
                    cache_.erase(it);
                    hash_.erase(*it);
                    return;
                }

                if (max < find->second[0]) {
                    elem = it;
                    max = find->second[0];
                }
            }
            cache_.erase(elem);
            hash_.erase(*elem);
        }

    private:
        size_type size_;
        std::unordered_map<KeyT, std::deque<size_type>> mp_;

        using ListIt = std::list<KeyT>::iterator;
        std::unordered_map<KeyT, ListIt> hash_;
        std::list<KeyT> cache_;
    };
}