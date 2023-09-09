#include "graph.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <regex>

using namespace containers;

std::vector<std::pair<int, int>> readInput() {
	std::vector<std::pair<int, int>> edges;
	std::regex pattern(R"(^\s*(\d*)\s*--\s*(\d*)\s*,\s*(\d*)\s*$)");
	std::string input;

	while (std::getline(std::cin, input)) {
		if (input.empty())
			continue;

		std::smatch matches;
		std::vector<int> values;
		if (std::regex_match(input, matches, pattern)) {
			for (int i = 1; i < matches.size(); ++i) {
				if (!matches.str(i).empty()) {
					int value = std::stoi(matches.str(i));
					values.push_back(value);
				}
			}
			edges.push_back({values[0], values[1]});
		} else {
			std::cout << "Expected input format: \"v -- u, w\"" << std::endl;
			exit(1);
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
			std::cout << color.cycle[i] + 1 << ' ';
		}
		std::cout << '\n';
	}
}