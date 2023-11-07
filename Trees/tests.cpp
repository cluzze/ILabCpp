#include <gtest/gtest.h>

#include "tree.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <random>
#include <iostream>
#include <fstream>

using namespace containers;

struct A {
    int x, y;
};

struct CompA {
    bool operator()(const A& a, const A&b) {
        return a.x < b.x;
    }
};

TEST(tree, constructor_non_trivial_class) {
    SearchTree<A, CompA> t;
    t.insert({1, 2});
    auto it = t.find({1, 2});
    ASSERT_TRUE(it != t.end());
}

TEST(tree, copy_ctr) {
    SearchTree<A, CompA> t;
    t.insert({1, 2});
    SearchTree<A, CompA> t2(t);
}

TEST(tree, iterators) {
    SearchTree<int> t;
    for (int i = 1; i <= 5; i++) {
        t.insert(i);
    }
    auto it = t.begin();
    for (int i = 1; i <= 5; i++, ++it) {
        ASSERT_EQ(it->key, i);
    }
}

TEST(tree, iterators_erase) {
    SearchTree<int> t;
    for (int i = 1; i <= 5; i++) {
        t.insert(i);
    }
    auto it = t.begin();
    auto it2 = (it++);
    t.erase(it);
    t.erase(it2);
    auto f = t.find(1);
    auto f2 = t.find(2);
    it = t.begin();
    ASSERT_EQ(f, t.end());
    ASSERT_EQ(f2, t.end());
    for (int i = 3; i <= 5; i++, ++it) {
        ASSERT_EQ(it->key, i);
    }
}

TEST(tree, next) {
    SearchTree<int> t;
    for (int i = 1; i <= 5; i++) {
        t.insert(i);
    }
    int i = 1;
#if 1
    auto it = t.begin();
#endif
    for (; it != t.end(); ++it) {
#if 0
    for (auto x : t) {
#endif
        if (i == 5)
            ASSERT_EQ(t.next(it), t.end());
        else
            ASSERT_EQ(t.next(it)->key, ++i);
    }
}