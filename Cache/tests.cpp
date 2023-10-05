#include <gtest/gtest.h>

#include "cache.hpp"
#include "lrucache.hpp"
#include "perfectcache.hpp"

#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <random>
#include <iostream>
#include <fstream>

using namespace caches;

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

TEST(cache, create) {
    std::vector<int> test{1, 2, 1, 2, 1, 2};
    lru_cache lru(2);
    lru_2_cache lru2(2);
    perfect_cache perf(2, test.begin(), test.end());

    ASSERT_FALSE(lru.full() && lru2.full() && perf.full());
}

TEST(cache, isPresent) {
    std::vector<int> test{1, 2, 1, 2, 1, 2};
    lru_cache lru(2);
    lru_2_cache lru2(2);
    perfect_cache perf(2, test.begin(), test.end());
    
    lru.lookup_update(1);
    lru2.lookup_update(1);
    perf.lookup_update(1);

    ASSERT_TRUE(lru.isPresent(1) && lru2.isPresent(1) && perf.isPresent(1));
}

TEST(cache, manual1) {
    std::vector<int> test{1, 2, 1, 2, 1, 2};
    lru_cache lru(2);
    lru_2_cache lru2(2);
    perfect_cache perf(2, test.begin(), test.end());
    int hits1 = 0, hits2 = 0, hits3 = 0;

    for (int i = 0; i < test.size(); i++) {
        int key = test[i];
        if (lru.lookup_update(key))
            hits1++;
        
        if (lru2.lookup_update(key))
            hits2++;
        
        if (perf.lookup_update(key))
            hits3++;
    }

    ASSERT_EQ(hits1, 4);
    ASSERT_EQ(hits2, 3);
    ASSERT_EQ(hits3, 4);
}

TEST(cache, manual2) {
    std::vector<int> test{1, 2, 3, 4, 5, 6, 7};
    lru_cache lru(3);
    lru_2_cache lru2(3);
    perfect_cache perf(3, test.begin(), test.end());
    int hits1 = 0, hits2 = 0, hits3 = 0;

    for (int i = 0; i < test.size(); i++) {
        int key = test[i];
        if (lru.lookup_update(key))
            hits1++;
        
        if (lru2.lookup_update(key))
            hits2++;
        
        if (perf.lookup_update(key))
            hits3++;
    }

    ASSERT_EQ(hits1, 0);
    ASSERT_EQ(hits2, 0);
    ASSERT_EQ(hits3, 0);
}

TEST(cache, manual3) {
    std::vector<int> test{1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4};
    lru_cache lru(4);
    lru_2_cache lru2(4);
    perfect_cache perf(4, test.begin(), test.end());
    int hits1 = 0, hits2 = 0, hits3 = 0;

    for (int i = 0; i < test.size(); i++) {
        int key = test[i];
        if (lru.lookup_update(key))
            hits1++;
        
        if (lru2.lookup_update(key))
            hits2++;
        
        if (perf.lookup_update(key))
            hits3++;
    }

    ASSERT_EQ(hits1, 6);
    ASSERT_EQ(hits2, 5);
    ASSERT_EQ(hits3, 6);
}

TEST(cache, manual4) {
    std::vector<int> test{4, 2, 1, 2, 5, 4, 1, 6, 3, 2, 10, 2, 9, 2, 7, 5, 10, 2, 6, 1, 0, 1, 2, 4, 10, 5, 9, 10, 2, 5};
    lru_cache lru(4);
    lru_2_cache lru2(4);
    perfect_cache perf(4, test.begin(), test.end());
    int hits1 = 0, hits2 = 0, hits3 = 0;

    for (int i = 0; i < test.size(); i++) {
        int key = test[i];
        if (lru.lookup_update(key))
            hits1++;
        
        if (lru2.lookup_update(key))
            hits2++;
        
        if (perf.lookup_update(key))
            hits3++;
    }

    ASSERT_EQ(hits1, 10);
    ASSERT_EQ(hits2, 10);
    ASSERT_EQ(hits3, 15);
}

std::vector<int> genTest(int n) {
    std::vector<int> result;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> v(1000, 50);
    for (int i = 1; i < v.size() - v.size() / 10; i++) {
        v[i] = i;
    }
    std::uniform_int_distribution<int> dist(0, v.size() - 1);

    for (int i = 0; i < n; ++i) {
        int index = dist(gen);
        result.push_back(v[index]);
    }

    return result;
}

TEST(cache, gen1) {
    std::vector<int> test = genTest(1000);
    lru_cache lru(10);
    lru_2_cache cache(10);
    perfect_cache perf(10, test.begin(), test.end());
    int hits1 = 0, hits2 = 0, hits3 = 0;

    for (int i = 0; i < test.size(); i++) {
        int key = test[i];
        if (lru.lookup_update(key))
            hits1++;
        
        if (cache.lookup_update(key))
            hits2++;
        
        if (perf.lookup_update(key))
            hits3++;
    }

    ASSERT_TRUE(hits3 >= hits1);
    ASSERT_TRUE(hits2 >= hits1);
    ASSERT_TRUE(hits3 >= hits2);
}

TEST(cache, gen2) {
    std::vector<int> test = genTest(1000);
    lru_cache lru(20);
    lru_2_cache cache(20);
    perfect_cache perf(20, test.begin(), test.end());
    int hits1 = 0, hits2 = 0, hits3 = 0;

    for (int i = 0; i < test.size(); i++) {
        int key = test[i];
        if (lru.lookup_update(key))
            hits1++;
        
        if (cache.lookup_update(key))
            hits2++;
        
        if (perf.lookup_update(key))
            hits3++;
    }

    ASSERT_TRUE(hits3 >= hits1);
    ASSERT_TRUE(hits2 >= hits1);
    ASSERT_TRUE(hits3 >= hits2);
}
