#include "graph.hpp"

#include <iostream>
#include <sstream>
#include <string>

using namespace containers;

std::vector<std::pair<int, int>> readInput() {
	std::vector<std::pair<int, int>> edges;
	std::string input;

	while (std::getline(std::cin, input)) {
		std::stringstream ss(input);

		int p, q, w;
		std::string d1, d2;
		
		ss >> p >> d1 >> q >> d2 >> w;

		if (d1 == "--" && d2 == "," && p > 0 && q > 0) {
			edges.push_back({p, q});
		}
		else {
			std::cerr << "Invalid input" << '\n';
		}
	}

	return edges;
}

int main() {
	std::vector<std::pair<int, int>> edges = readInput();

	Graph g(edges);

	if (g.dfs())
		g.printColor();
	else
		std::cout << "graph is not bipartite" << '\n';
}