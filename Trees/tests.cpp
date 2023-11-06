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
    t.insert(1);
    t.insert(2);
    t.insert(3);
    t.insert(4);
    auto it = t.begin();
    it++;
    ASSERT_TRUE(it->key);
}