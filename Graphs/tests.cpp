#include <gtest/gtest.h>

#include <vector>
#include <algorithm>
#include <cstdlib>

#include "graph.hpp"

using std::vector;
using std::pair;

bool dfs_impl(int i, int color, std::vector<int>& used, vector<vector<int>>& graph) {
    used[i] = color;
    for (int v : graph[i]) {
        if (used[v] == color)
            return false;
        
        if (used[v] == -1)
            if (!dfs_impl(v, color ^ 1, used, graph))
                return false;	
    }
    return true;
}

bool dfs(int v, vector<vector<int>>& graph) {
    int color = 0;
    std::vector<int> used(v, -1);
    
    for (int i = 0; i < v; i++)
        if (used[i] == -1) {
            if (!dfs_impl(i, color, used, graph))
                return false;
        }

    return true;
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

TEST(graph, hand_written1) {
    vector<pair<int, int>> edges;
    edges.push_back({1, 6});
    edges.push_back({1, 5});
    edges.push_back({1, 5});
    edges.push_back({2, 6});
    edges.push_back({2, 6});
    edges.push_back({2, 4});
    edges.push_back({3, 4});
    edges.push_back({3, 4});
    edges.push_back({3, 4});

    vector<vector<int>> graph(9);

    for (int i = 0; i < edges.size(); i++) {
        graph[edges[i].first - 1].push_back(edges[i].second - 1);
        graph[edges[i].second - 1].push_back(edges[i].first - 1);
    }

    containers::Graph mygraph(edges);

    bool res1 = false, res2 = false, res3 = false;

    res1 = dfs(9, graph);
    res2 = mygraph.dfs();
    res3 = mygraph.bfs();

    ASSERT_EQ(res1, res2);
    ASSERT_EQ(res1, res3);
}

TEST(graph, hand_written2) {
    vector<pair<int, int>> edges;
    edges.push_back({1, 6});
    edges.push_back({1, 9});
    edges.push_back({2, 8});
    edges.push_back({3, 6});
    edges.push_back({3, 7});
    edges.push_back({4, 8});
    edges.push_back({4, 9});
    edges.push_back({5, 9});

    vector<vector<int>> graph(100);

    for (int i = 0; i < edges.size(); i++) {
        graph[edges[i].first - 1].push_back(edges[i].second - 1);
        graph[edges[i].second - 1].push_back(edges[i].first - 1);
    }

    containers::Graph mygraph(edges);

    bool res1 = false, res2 = false, res3 = false;

    res1 = dfs(9, graph);
    res2 = mygraph.dfs();
    res3 = mygraph.bfs();

    ASSERT_EQ(res1, res2);
    ASSERT_EQ(res1, res3);
}

TEST(graph, hand_written3) {
    vector<pair<int, int>> edges;
    edges.push_back({1, 2});
    edges.push_back({2, 3});
    edges.push_back({1, 3});

    vector<vector<int>> graph(100);

    for (int i = 0; i < edges.size(); i++) {
        graph[edges[i].first - 1].push_back(edges[i].second - 1);
        graph[edges[i].second - 1].push_back(edges[i].first - 1);
    }

    containers::Graph mygraph(edges);

    bool res1 = false, res2 = false, res3 = false;

    res1 = dfs(9, graph);
    res2 = mygraph.dfs();
    res3 = mygraph.bfs();

    ASSERT_EQ(res1, res2);
    ASSERT_EQ(res1, res3);
}

TEST(graph, generated1) {
    srand(time(NULL));
    int size = 100;
    vector<int> v1(size), v2(size);

    for (int i = 0; i < v1.size(); i++) {
        v1[i] = i + 1;
    }
    
    for (int i = 0; i < v2.size(); i++) {
        v2[i] = i + size + 1; 
    }
    
    vector<pair<int, int>> edges;

    for (int i = 0; i < v1.size(); i++) {
        int e1 = 0, e2 = 0, e3 = 0;
        e1 = std::rand() % size;
        e2 = std::rand() % size;
        e3 = std::rand() % size;

        edges.push_back({v1[i], v2[e1]});
        edges.push_back({v1[i], v2[e2]});
        edges.push_back({v1[i], v2[e3]});
    }

    vector<vector<int>> graph(size * 2);

    for (int i = 0; i < edges.size(); i++) {
        graph[edges[i].first - 1].push_back(edges[i].second - 1);
        graph[edges[i].second - 1].push_back(edges[i].first - 1);
    }


    containers::Graph mygraph(edges);

    bool res1 = false, res2 = false, res3 = false;

    res1 = dfs(size * 2, graph);
    res2 = mygraph.dfs();
    res3 = mygraph.bfs();

    ASSERT_EQ(res1, res2);
    ASSERT_EQ(res1, res3);
}

TEST(graph, generated2) {
    srand(time(NULL));
    int size = 100;
    vector<int> v1(size), v2(size);

    for (int i = 0; i < v1.size(); i++) {
        v1[i] = i + 1;
    }
    
    for (int i = 0; i < v2.size(); i++) {
        v2[i] = i + size + 1; 
    }
    
    vector<pair<int, int>> edges;

    for (int i = 0; i < v1.size(); i++) {
        int e1 = 0, e2 = 0, e3 = 0;
        e1 = std::rand() % size;
        e2 = std::rand() % size;
        e3 = std::rand() % size;

        edges.push_back({v1[i], v2[e1]});
        edges.push_back({v1[i], v2[e2]});
        edges.push_back({v1[i], v2[e3]});
    }

    edges.push_back({1, 1});

    vector<vector<int>> graph(size * 2);

    for (int i = 0; i < edges.size(); i++) {
        graph[edges[i].first - 1].push_back(edges[i].second - 1);
        graph[edges[i].second - 1].push_back(edges[i].first - 1);
    }

    containers::Graph mygraph(edges);

    bool res1 = false, res2 = false, res3 = false;

    res1 = dfs(size * 2, graph);
    res2 = mygraph.dfs();
    res3 = mygraph.bfs();

    ASSERT_EQ(res1, res2);
    ASSERT_EQ(res1, res3);
}