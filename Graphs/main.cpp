#include "graph.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <regex>

using namespace containers;

std::vector<std::pair<int, int>> readInput() {
	std::vector<std::pair<int, int>> edges;
	std::regex inputPattern(R"((\d+) -- (\d+), (\d+))");
	std::string input;

	while (std::getline(std::cin, input)) {
		std::smatch matches;

        if (std::regex_match(input, matches, inputPattern)) {
            int p = std::stoi(matches[1]);
            int q = std::stoi(matches[2]);
            int w = std::stoi(matches[3]);

            if (p > 0 && q > 0) {
                edges.push_back({p, q});
            } else {
                std::cerr << "Invalid input" << '\n';
            }
        } else {
            std::cerr << "Invalid input" << '\n';
        }
	}

	return edges;
}

int main() {
	std::vector<std::pair<int, int>> edges = readInput();

	Graph g(edges.begin(), edges.end());
	ColorResult color = g.bfs();
	if (color.res)
		g.printColor();
	else {
		std::cout << "graph contains cycle of odd length:" << '\n';
		for (int i = 0; i < color.cycle.size(); i++) {
			std::cout << i + 1 << ' ';
		}
		std::cout << '\n';
	}
}