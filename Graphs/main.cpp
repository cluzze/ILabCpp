#include "graph.hpp"

using namespace containers;

int main() {
	std::vector<std::pair<int, int>> edges;
	int p, q;
	while (std::cin >> p >> q) {
		edges.push_back({p, q});
	}

	Graph g(edges);

	if (g.dfs())
		g.printColor();
	else
		std::cout << "graph is not bipartite" << '\n';
}