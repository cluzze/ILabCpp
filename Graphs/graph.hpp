/*Implementation of the Graph desrcibed in TAOCP 7.2.1*/

#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <queue>

namespace containers
{
	struct ColorResult {
		bool res;
		std::vector<int> cycle;
	};
	class Graph {
	public:
		template <typename It>
		Graph(It begin, It end) ;

		Graph(std::initializer_list<std::pair<int, int>> init);

		void dump() const;

		void printColor() const;

		/*Addes adge between two existing vertices*/
		void addEdge(int p, int q);

		/*2-colors graph if possible using dfs*/
		ColorResult dfs();

		/*2-colors graph if possible using bfs*/
		ColorResult bfs();

		void clearColor();

	private:
		void Unlink(int taget);

		void LinkBefore(int target, int before);

		bool dfs_impl(int i, std::vector<int>& cycle);

		bool bfs_impl(int i, std::vector<int>& cycle);

		void findCycle();

		struct EdgeInfo {
			int ta, na, pa;
			int color;
			int parent;
			int link;
		};

		int v = 0, e = 0;
		std::vector<EdgeInfo> graph;
	};

    void Graph::Unlink(int target) {
		int before = graph[target].pa;
		int after = graph[target].na;
		graph[before].na = after;
		graph[after].pa = before;
    }

    void Graph::LinkBefore(int target, int before)
    {
        graph[before].na = target;
		graph[before].pa = graph[target].pa;
		graph[graph[target].pa].na = before;
    	graph[target].pa = before;
    }

    template <typename It>
	Graph::Graph(It begin, It end) {
		for (auto it = begin; it != end; ++it) {
			v = std::max({v, it->first, it->second});
		}

		graph.resize(v, { 0, 0, 0, -1, -1, -1 });

		for (int i = 0; i < v; i++) {
			graph[i].na = i;
        	graph[i].pa = i;
		}
		
		for (auto it = begin; it != end; ++it) {
			int p = it->first;
			int q = it->second;
			
			addEdge(p, q);
		}
	}

	Graph::Graph(std::initializer_list<std::pair<int, int>> init) : 
		Graph(init.begin(), init.end()) {}

	void Graph::addEdge(int p, int q) {
		int ep = 2 * e + v;
		int eq = 2 * e + 1 + v;

		graph.push_back({p - 1, 0, 0, -1});
		graph.push_back({q - 1, 0, 0, -1});

		LinkBefore(p - 1, eq);
		LinkBefore(q - 1, ep);
		e++;
	}

	ColorResult Graph::bfs() {
		int color = 0;
		ColorResult res;

		res.res = false;
		for (int i = 0; i < v; i++)
			if ((!bfs_impl(i, res.cycle))) {
				clearColor();
				return res;
			}

		std::for_each(graph.begin(), graph.begin() + v, [](EdgeInfo& v) {
        	v.color %= 2;
    	});
		
		res.res = true;
		return res;
	}

	bool Graph::bfs_impl(int i, std::vector<int>& cycle) {
		for (int e = graph[i].na; e != i; e = graph[e].na) {
			int next = graph[e].ta;

			if ((graph[next].color != -1) && graph[next].color % 2 == graph[i].color % 2) {
				clearColor();
				cycle = dfs().cycle;
				return false;
			}

			graph[next].color = std::max(graph[next].color, graph[i].color + 1);
		}

		return true;
	}
	
	ColorResult Graph::dfs() {
		int color = 0;
		ColorResult res;

		res.res = true;

		for (int i = v - 1; i >= 0; i--) {
			if ((graph[i].color == -1) && !(dfs_impl(i, res.cycle))) {
				res.res = false;
				break;
			}
		}
		
		return res;
	}

	bool Graph::dfs_impl(int i, std::vector<int>& cycle) {
		bool res = true;
		int v1, v2;
		int s;
		int a;

		graph[i].color = 0;
		graph[i].parent = i;
		graph[i].link = -1; //??
		s = i;

		while (s != -1) {
			v1 = s;
			s = graph[s].link;
			a = graph[v1].na;
			
			for (a = graph[v1].na; a != v1; a = graph[a].na) {
				v2 = graph[a].ta;
				if (graph[v2].color == -1) {
					graph[v2].parent = v1;
					graph[v2].color = 1 - graph[v1].color;
					graph[v2].link = s;
					s = v2;
				} else if (graph[v1].color == graph[v2].color) {
					while (v1 != graph[v2].parent) {
						cycle.push_back(v1);
						//std::cout << v1 << ' ';
						v1 = graph[v1].parent;
					}
					cycle.push_back(v1);
					cycle.push_back(v2);
					//std::cout << v1 << ' ' << v2 << '\n';
					return false;
				}
			}
		}
		return true;
	}

	void Graph::clearColor() {
		for (auto& it : graph) {
			it.color = -1;
		}
	}

	void Graph::dump() const {
		for (int i = 0; i < v + 2 * e; i++) {
			std::cout << i << ' ' << graph[i].ta << ' ' << graph[i].na << ' ' << graph[i].pa << '\n';
		}
	}

	void Graph::printColor() const {
		for (int i = 0; i < v; i++) {
			char color = graph[i].color ? 'r' : 'b';
			std::cout << i + 1 << ' ' << color << ' ';
		}
		std::cout << '\n';
	}
}