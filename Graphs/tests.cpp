#include <gtest/gtest.h>

#include <vector>
#include <algorithm>
#include <cstdlib>
#include <random>

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

bool dfs(vector<vector<int>>& graph) {
    int color = 0;
    int v = graph.size();
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

    res1 = dfs(graph);
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

    vector<vector<int>> graph(9);

    for (int i = 0; i < edges.size(); i++) {
        graph[edges[i].first - 1].push_back(edges[i].second - 1);
        graph[edges[i].second - 1].push_back(edges[i].first - 1);
    }

    containers::Graph mygraph(edges);

    bool res1 = false, res2 = false, res3 = false;

    res1 = dfs(graph);
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

    vector<vector<int>> graph(3);

    for (int i = 0; i < edges.size(); i++) {
        graph[edges[i].first - 1].push_back(edges[i].second - 1);
        graph[edges[i].second - 1].push_back(edges[i].first - 1);
    }

    containers::Graph mygraph(edges);

    bool res1 = false, res2 = false, res3 = false;

    res1 = dfs(graph);
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

    res1 = dfs(graph);
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

    res1 = dfs(graph);
    res2 = mygraph.dfs();
    res3 = mygraph.bfs();

    ASSERT_EQ(res1, res2);
    ASSERT_EQ(res1, res3);
}

std::vector<std::pair<int, int>> generateBigBipartiteGraph(int numNodes1, int numNodes2, int numEdges) {
    std::vector<std::pair<int, int>> graph;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist1(1, numNodes1);
    std::uniform_int_distribution<int> dist2(numNodes1 + 1, numNodes1 + numNodes2);

    for (int i = 0; i < numEdges; i++) {
        int node1 = dist1(gen);
        int node2 = dist2(gen);
        graph.push_back({node1, node2});
    }

    return graph;
}

TEST(graph, generated3) {
    int numNodes1 = 100000;
    int numNodes2 = 15000;
    int numEdges = 500000;

    std::vector<std::pair<int, int>> bipartiteGraph = generateBigBipartiteGraph(numNodes1, numNodes2, numEdges);

    containers::Graph mygraph(bipartiteGraph);

    bool res1 = false, res2 = false, res3 = false;

    vector<vector<int>> graph(numNodes1 + numNodes2);

    for (int i = 0; i < bipartiteGraph.size(); i++) {
        graph[bipartiteGraph[i].first - 1].push_back(bipartiteGraph[i].second - 1);
        graph[bipartiteGraph[i].second - 1].push_back(bipartiteGraph[i].first - 1);
    }

    res1 = dfs(graph);
    res2 = mygraph.dfs();
    res3 = mygraph.bfs();
}
