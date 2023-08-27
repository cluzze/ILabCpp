/*Implementation of the Graph desrcibed in TAOCP 7.2.1*/

#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <queue>

namespace containers
{
	class Graph {
	public:
		Graph(std::vector<std::pair<int, int>> edges);

		Graph(std::initializer_list<std::pair<int, int>> init);

		void dump();

		void printColor();

		/*2-colors graph if possible using dfs*/
		bool dfs();

		/*2-colors graph if possible using bfs*/
		bool bfs();

	private:
		void LinkBefore(int target, int before);

		bool dfs_impl(int i, int color, std::vector<int>& used);

		bool bfs_impl(int i, std::vector<int>& used);

		struct EdgeInfo {
			int ta, na, pa;
			int color;
		};

		int v{}, e{};
		std::vector<EdgeInfo> graph{};
	};

	void Graph::LinkBefore(int target, int before) {
		graph[before].na = target;
		graph[before].pa = graph[target].pa;
		graph[graph[target].pa].na = before;
    	graph[target].pa = before;
	}

	Graph::Graph(std::vector<std::pair<int, int>> init) {

		e = init.size();
		v = 0;

		for (const auto& p : init) {
			v = std::max(v, std::max(p.first, p.second));
		}

		graph.resize(v + 2 * e, { 0, 0, 0, 0 });

		for (int i = 0; i < v; i++) {
			graph[i].na = i;
        	graph[i].pa = i;
		}

		int i = 0;
		
		for (const auto& it : init) {
			int p = it.first;
			int q = it.second;
			
			int ep = 2 * i + v;
			int eq = 2 * i + 1 + v;

			graph[ep].ta = p - 1;
			graph[eq].ta = q - 1;

			LinkBefore(p - 1, eq);
			LinkBefore(q - 1, ep);
			i++;
		}
	}

	Graph::Graph(std::initializer_list<std::pair<int, int>> init) : 
		Graph(std::vector<std::pair<int, int>>(init.begin(), init.end())) {}

	bool Graph::bfs() {
		int color = 0;
		std::vector<int> used(v, -1);
		
		for (int i = 0; i < v; i++)
			if (used[i] == -1)
				if (!bfs_impl(i, used))
					return false;
		
		for (int i = 0; i < v; i++)
			graph[i].color = used[i];

		return true;
	}

	bool Graph::bfs_impl(int i, std::vector<int>& used) {
		std::queue<int> q;
		q.push(i);
		used[i] = 0;

		while (!q.empty()) {
			int v = q.front();
			q.pop();
			for (int e = graph[v].na; e != v; e = graph[e].na) {
				if (used[graph[e].ta] == used[v])
					return false;
			
				if (used[graph[e].ta] == -1) {
					used[graph[e].ta] = used[v] ^ 1;
					q.push(graph[e].ta);
				}
			}
		}

		return true;
	}
	
	bool Graph::dfs() {
		int color = 0;
		std::vector<int> used(v, -1);
		
		for (int i = 0; i < v; i++)
			if (used[i] == -1)
				if (!dfs_impl(i, color, used))
					return false;
				
		for (int i = 0; i < v; i++)
			graph[i].color = used[i];

		return true;
	}

	bool Graph::dfs_impl(int i, int color, std::vector<int>& used) {
		used[i] = color;
		for (int e = graph[i].na; e != i; e = graph[e].na) {
			if (used[graph[e].ta] == color)
				return false;
			
			if (used[graph[e].ta] == -1)
				if (!dfs_impl(graph[e].ta, color ^ 1, used))
					return false;	
		}
		return true;
	}

	void Graph::dump() {
		for (int i = 0; i < v + 2 * e; i++) {
			std::cout << i << ' ' << graph[i].ta + 1 << ' ' << graph[i].na << ' ' << graph[i].pa << '\n';
		}
	}

	void Graph::printColor() {
		for (int i = 0; i < v; i++) {
			char color = graph[i].color ? 'r' : 'b';
			std::cout << i + 1 << ' ' << color << ' ';
		}
		std::cout << '\n';
	}
}