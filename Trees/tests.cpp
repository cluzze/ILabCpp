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
    bool operator()(const A& a, const A&b) const {
        return a.x < b.x;
    }
};

template class SearchTree<A, CompA>; // explicit instantions for correctness check

bool cmp(const A& a, const A&b) {
    return a.x < b.x;
}

TEST(tree, constructor_non_trivial_class) {
    SearchTree<A, CompA> t;
    t.insert({1, 2});
    ASSERT_TRUE(t.size() == 1);
    auto it = t.find({1, 2});
    ASSERT_TRUE(it != t.end());
}

TEST(tree, copy_ctr) {
    SearchTree<A, CompA> t;
    t.insert({1, 2});
    ASSERT_TRUE(t.size() == 1);
    SearchTree<A, CompA> t2(t);
    auto f = [](const A& a, const A&b) {
        return a.x < b.x;
    };
    SearchTree<A, decltype(f)> tree(f);
    ASSERT_TRUE(tree.empty());
    SearchTree<A, bool (*)(const A&, const A&)> tree2(cmp);
    ASSERT_TRUE(tree2.empty());
}

TEST(tree, iterators) {
    SearchTree<int> t;
    for (int i = 1; i <= 5; i++) {
        t.insert(i);
        ASSERT_TRUE(t.size() == i);
    }
    auto it = t.begin();
    for (int i = 1; i <= 5; i++, ++it) {
        ASSERT_EQ(*it, i);
    }
}

TEST(tree, iterators_erase) {
    SearchTree<int> t;
    for (int i = 1; i <= 5; i++) {
        t.insert(i);
        ASSERT_TRUE(t.size() == i);
    }
    auto it = t.begin();
    auto it2 = std::next(it);
    t.erase(it);
    ASSERT_TRUE(t.size() == 4);
    t.erase(it2);
    ASSERT_TRUE(t.size() == 3);
    t.insert(5);
    ASSERT_TRUE(t.size() == 3);
    auto f = t.find(1);
    auto f2 = t.find(2);
    it = t.begin();
    ASSERT_EQ(f, t.end());
    ASSERT_EQ(f2, t.end());
    for (int i = 3; i <= 5; i++, ++it) {
        ASSERT_EQ(*it, i);
    }
}

TEST(tree, next) {
    SearchTree<int> t;
    for (int i = 1; i <= 5; i++) {
        t.insert(i);
    }
    int i = 1;
    auto it = t.begin();
    for (; it != t.end(); ++it) {
        if (i == 5)
            ASSERT_EQ(t.next(it), t.end());
        else
            ASSERT_EQ(*t.next(it), ++i);
    }

    it = t.begin();
    //*it = 123; CE
}

TEST(tree, correct_order) {
    SearchTree<int> t;
    std::vector<int> v(100);
    int k = 1;
    std::random_device rd;
    std::mt19937 g(rd());
    std::generate(v.begin(), v.end(), [&k](){ return k++; });
    std::shuffle(v.begin(), v.end(), g);
    std::for_each(v.begin(), v.end(), [&](const auto& x) { t.insert(x); });
    auto order = t.getInorder();
    ASSERT_TRUE(std::is_sorted(order.begin(), order.end()));
    for (int i = 1; i <= v.size() / 2; i++) {
        auto it = t.find(i);
        t.erase(it);
        ASSERT_TRUE(std::is_sorted(order.begin(), order.end()));
    }
}
