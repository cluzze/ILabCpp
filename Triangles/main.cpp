#include "triangles.hpp"
#include "broadphase.hpp"

#include <iostream>
#include <random>
#include <set>
#include <ctime>
#include <fstream>

using namespace geometry;

int main() {
	int n;
	std::cin >> n;
	std::vector<Triangle3> tri;
	Vec3 min{1e9, 1e9, 1e9}, max{-1e9, -1e9, -1e9};
	for (int i = 0; i < n; i++) {
		std::vector<Vec3> v(3);
		for (int j = 0; j < 3; j++) {
			std::cin >> v[j][0] >> v[j][1] >> v[j][2];
		}
		tri.push_back({v[0], v[1], v[2]});

		BoundingBox aabb(v[0], v[1], v[2]);
		for (int j = 0; j < 3; j++) {
			if (min[j] > aabb.get_min()[j])
				min[j] = aabb.get_min()[j];
			if (max[j] < aabb.get_max()[j])
				max[j] = aabb.get_max()[j];
		}
	}

	geometry::BoundingBox box(min, max);
	broadphase::octree tree(5, box, tri.begin(), tri.end());
	//tree.dump();

	std::set<int> collisions_smart;

	//std::clock_t start = std::clock();
	for (int i = 0; i < n; i++) {
		std::vector<std::size_t> v = tree.get_potential_collision(i);
		for (int j = 0; j < v.size(); j++) {
			int res = tri[i].intersect(tri[v[j]]);
			if (res > 0) {			
				collisions_smart.insert(i);
				collisions_smart.insert(v[j]);
			}
		}
	}
	//std::clock_t fin = std::clock();
	
	for (auto x : collisions_smart)
		std::cout << x << '\n';

	//std::cout << "\ntime took:" << 1000.0 * (fin - start) / CLOCKS_PER_SEC << "ms\n";
}