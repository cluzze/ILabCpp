#include "triangles.hpp"
#include "broadphase.hpp"

#include <iostream>
#include <random>
#include <fstream>

using namespace geometry;

std::vector<face_t> generateRandomTriangles(int n) {
    std::vector<face_t> triangles;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-10.0, 10.0);

	std::vector<point3_t> v(3);

	std::ofstream file;
	file.open("tests/test1.txt");
	file << n << '\n';

    for (int i = 0; i < n; ++i) {
		for (int j = 0; j < 3; j++) {
			v[j] = {dist(gen), dist(gen), dist(gen)};
			file << v[j][0] << " " << v[j][1] << " " << v[j][2];
		}
		file << '\n';

        face_t triangle(v.begin(), v.end());
        triangles.push_back(triangle);
    }
	file.close();
	
    return triangles;
}

int main()
{
	int n;
	std::ifstream file;
	file.open("tests/data.txt");
	file >> n;
	std::vector<face_t> faces;
	point3_t min(1e9, 1e9, 1e9), max(-1e9, -1e9, -1e9);
	for (int i = 0; i < n; i++) {
		std::vector<point3_t> v(3);
		for (int j = 0; j < 3; j++) {
			file >> v[j][0] >> v[j][1] >> v[j][2];
			std::cout << v[j][0] << ' ' <<  v[j][1] << ' ' << v[j][2] << '\n';
		}
		faces.push_back({v.begin(), v.end()});

		bounding_box_t aabb(v.begin(), v.end());
		for (int j = 0; j < 3; j++) {
			if (min[j] > aabb.get_min()[j])
				min[j] = aabb.get_min()[j];
				
			if (max[j] < aabb.get_max()[j])
				max[j] = aabb.get_max()[j];
		}
	}

	geometry::bounding_box_t box(min - point3_t{1, 1, 1}, max + point3_t{1, 1, 1});
	broadphase::octree tree(3, box, faces.begin(), faces.end());
	tree.dump();

	// std::vector<int> used(n, 0);
	// for (int i = 0; i < n; i++) {
	// 	for (int j = i; j < n; j++) {
	// 		int res = trianglesIntersection(faces[i], faces[j]);
	// 		if (res > 0) {
	// 			if (!used[i]) {
	// 				std::cout << i;
	// 				used[i] = 1;
	// 			}
	// 			if (!used[j]) {
	// 				std::cout << j;
	// 				used[j] = 1;
	// 			}
	// 		}
	// 	}
	// }
}