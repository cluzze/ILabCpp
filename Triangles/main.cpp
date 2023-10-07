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
    std::uniform_real_distribution<double> dist(0.0, 1.0);

	std::vector<point3_t> v(3);

	std::ofstream file;
	file.open("tests/test1.txt");
	file << n;

    for (int i = 0; i < n; ++i) {
		
		for (int j = 0; j < 3; j++) {
			v[j] = {dist(gen), dist(gen), dist(gen)};
			file << v[j][0] << v[j][1] << v[j][2];
		}

        face_t triangle(v.begin(), v.end());
        triangles.push_back(triangle);
    }
	file.close();
	
    return triangles;
}

int main()
{
	int n;
	std::cin >> n;
	std::vector<face_t> faces;
	for (int i = 0; i < n; i++) {
		std::vector<point3_t> v(3);
		std::cin >> v[i][0] >> v[i][1] >> v[i][2];
		faces.push_back({v.begin(), v.end()});
	}
	
	std::vector<int> used(n, 0);
	for (int i = 0; i < n; i++) {
		for (int j = i; j < n; j++) {
			int res = trianglesIntersection(faces[i], faces[j]);
			if (res > 0) {
				if (!used[i]) {
					std::cout << i;
					used[i] = 1;
				}
				if (!used[j]) {
					std::cout << j;
					used[j] = 1;
				}
			}
		}
	}
}