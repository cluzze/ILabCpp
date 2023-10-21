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

		void clearFields();

	private:
		void Unlink(int taget);

		void LinkBefore(int target, int before);

		bool dfs_impl(int i, std::vector<int>& cycle);

		bool bfs_impl(int i, std::vector<int>& cycle);

		void saveCycle(int v1, int v2, std::vector<int>& cycle);

		bool visitAdjacentNodes(int& s1, int& s2, std::vector<int>& cycle, int pop, int push); 
		struct EdgeInfo {
			int ta, na, pa;
			int color;
			int parent;
			int link;
			int link2;
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

		graph.resize(v, { 0, 0, 0, -1, -1, -1, -1 });

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

		graph.push_back({p - 1, 0, 0, -1, -1, -1, -1});
		graph.push_back({q - 1, 0, 0, -1, -1, -1, -1});

		LinkBefore(p - 1, eq);
		LinkBefore(q - 1, ep);
		e++;
	}

	void Graph::saveCycle(int v1, int v2, std::vector<int>& cycle) {
		for (int i = 0; i < v; i++)
			graph[i].link = -1;

		int cycleSize = 0;
		int old_v2 = v2;
		graph[v2].link = 1;
		while (v2 != graph.at(v2).parent) {
			graph[v2].link = 1;
			v2 = graph[v2].parent;
		}
		graph[v2].link = 1;
		while (graph[v1].link == -1) {
			cycleSize++;
			cycle.push_back(v1);
			v1 = graph[v1].parent;
		}
		int size = cycle.size() + cycleSize + 1;
		cycle.resize(size);
		v2 = old_v2;
		for (int i = size - 1; i >= size - cycleSize - 1; i--) {
			cycle[i] = v2;
			v2 = graph[v2].parent;
		}
	}

	bool Graph::visitAdjacentNodes(int& s1, int& s2, std::vector<int>& cycle, int pop, int push) {
		int v1;
		if (pop == 1) {
			v1 = s1;
			s1 = graph[s1].link;
		} else {
			v1 = s2;
			s2 = graph[s2].link2;
		}

		int a = graph[v1].na;
		for (a = graph[v1].na; a != v1; a = graph[a].na) {
			int v2 = graph[a].ta;
			if (v1 == v2) {
				cycle.push_back(v1);
				return false;
			}
			if (graph[v2].color == -1) {
				graph[v2].parent = v1;
				graph[v2].color = 1 - graph[v1].color;
				if (push == 1) {
					graph[v2].link = s1;
					s1 = v2;
				} else {
					graph[v2].link2 = s2;
					s2 = v2;
				}
			} else if (graph[v1].color == graph[v2].color) {
				//saveCycle(v1, v2, cycle);
				return false;
			}
		}
		return true;
	}

	ColorResult Graph::bfs() {
		ColorResult res;

		res.res = false;
		for (int i = v - 1; i >= 0; i--)
			if ((graph[i].color == -1) && (!bfs_impl(i, res.cycle))) {
				clearFields();
				return res;
			}
		
		res.res = true;
		return res;
	}

	bool Graph::bfs_impl(int i, std::vector<int>& cycle) {
		int s1, s2;

		graph[i].color = 0;
		graph[i].parent = i;
		graph[i].link = -1;
		graph[i].link2 = -1;

		s1 = i;
		s2 = -1;

		while (s1 != -1 || s2 != -1) {
			if ((s1 != -1) && (!visitAdjacentNodes(s1, s2, cycle, 1, 2))) {
				return false;
			} else if ((s2 != -1) && !visitAdjacentNodes(s1, s2, cycle, 2, 1)) {
				return false;
			}
		}

		return true;
	}
	
	ColorResult Graph::dfs() {
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
		int s;

		graph[i].color = 0;
		graph[i].parent = i;
		graph[i].link = -1;
		s = i;

		while (s != -1)
			if (!visitAdjacentNodes(s, s, cycle, 1, 1))
				return false;

		return true;
	}

	void Graph::clearFields() {
		for (auto& it : graph) {
			it.color = -1;
			it.link = -1;
			it.link2 = -1;
			it.parent = -1;
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