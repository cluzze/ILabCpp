#pragma once

#include <array>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <list>

namespace geometry
{
	struct point3_t {
	public:
		point3_t(double x = 0, double y = 0, double z = 0) : p{x, y, z} {}

		double& operator[](size_t id) & { return p[id]; }
		const double& operator[](size_t id) const & { return p[id]; }

	private:
		std::array<double, 3> p;
	};

	point3_t cross(const point3_t& a, const point3_t& b);

	double dot(const point3_t& a, const point3_t& b) {
		double res = 0;
		for (int i = 0; i < 3; i++)
			res += a[i] * b[i];
		return res;
	}

	point3_t operator+(const point3_t& a, const point3_t& b) {
		return {a[0] + b[0], a[1] + b[1], a[2] + b[2]};
	}

	point3_t operator-(const point3_t& a) {
		return {-a[0], -a[1], -a[2]};
	}

	point3_t operator-(const point3_t& a, const point3_t& b) {
		return a + (-b);
	}

	point3_t operator/(const point3_t& a, double x) {
		return {a[0] / x, a[1] / x, a[2] / x};
	}
	

	struct bounding_box_t {
	private:
		point3_t min, max;
	
	public:
		bounding_box_t(const point3_t& a, const point3_t& b) : 
			min{std::min(a[0], b[0]), std::min(a[1], b[1]), std::min(a[2], b[2])}, 
			max{std::max(a[0], b[0]), std::max(a[1], b[1]), std::max(a[2], b[2])} {}

		template <typename It>
		bounding_box_t(It begin, It end) {
			for (int i = 0; i < 3; i++) {
				min[i] = (*std::min_element(begin, end, [=](const point3_t& x, const point3_t& y) {
					return x[i] < y[i];
				}))[i];

				max[i] = (*std::max_element(begin, end, [=](const point3_t& x, const point3_t& y) {
					return x[i] < y[i];
				}))[i];
			}
		}

		bool isInsideBoundingBox(const bounding_box_t& other) const {
			point3_t other_min = other.get_min();
			point3_t other_max = other.get_max();

			return ((max[0] < other_max[0]) &&
					(max[1] < other_max[1]) &&
					(max[2] < other_max[2]) &&
					(min[0] > other_min[0]) &&
					(min[1] > other_min[1]) &&
					(min[2] > other_min[2]));
		}

		bool boundingBoxIntersect(const bounding_box_t& other) const {
			point3_t other_min = other.get_min();
			point3_t other_max = other.get_max();
			return !((min[0] > other_max[0]) ||
					(min[1] > other_max[1]) ||
					(min[2] > other_max[2]) ||
					(max[0] < other_min[0]) ||
					(max[1] < other_min[1]) ||
					(max[2] < other_min[2]));
		}

		point3_t get_min() const { return min; }
		point3_t get_max() const { return max; }

		point3_t get_center() const { return (min + max) / 2; }
	};

	struct edge_t {
		edge_t(const point3_t& a_, const point3_t& b_) : a{a_}, b{b_}, box(a, b) {}

		point3_t a, b;

	private:
		bounding_box_t box;
	};

	struct face_t {

		bool pointInsideTrianle(const point3_t& p) const;

		void rotateFaceNodes(int n);

		void swapFaceNodes(int a, int b);

		point3_t getNormal() const;

		template <typename It>
		face_t(It begin, It end) : points(begin, end), box(begin, end), normal{getNormal()} {}

		face_t(std::initializer_list<point3_t> init) : face_t(init.begin(), init.end()) {}

		point3_t& operator[](size_t id) & { return points[id]; }
		const point3_t& operator[](size_t id) const & { return points[id]; }

		size_t n_points() const { return points.size(); }

		bounding_box_t get_box() const { return box; }

		private:
			std::vector<point3_t> points;
			bounding_box_t box;
			point3_t normal;
	};

	void face_t::rotateFaceNodes(int n) {
		point3_t temp;
		for (int i = 0; i < n; i++) {
			temp = points[points.size() - 1];
			for (int j = points.size() - 1; j >= 1; j--){
				points[j] = points[j - 1];
			}
			points[0] = temp;
		}
	}

	point3_t face_t::getNormal() const {
		point3_t p1 = points[1] - points[0];
		point3_t p2 = points[2] - points[0];
		return cross(p1, p2);
	}

	void face_t::swapFaceNodes(int a, int b) {
		std::swap(points[a], points[b]);
		normal = -normal;
		return;
	}

	bool face_t::pointInsideTrianle(const point3_t& p) const {
		point3_t c1 = cross({points[0] - points[1]}, normal);
		point3_t c2 = cross({points[1] - points[2]}, normal);
		point3_t c3 = cross({points[2] - points[0]}, normal);

		double x = dot(points[0], c1);
		double y = dot(points[1], c2);
		double z = dot(points[2], c3);

		if ((x > 0 && y > 0 && z > 0) || (x < 0 && y < 0 && z < 0))
			return 1;
		
		return 0;
	}


	double pointDeterminant(const point3_t& a, const point3_t& b, const point3_t& c, const point3_t& d) {
		double a1 = d[0] - a[0];
		double a2 = d[0] - b[0];
		double a3 = d[0] - c[0];
		
		double b1 = d[1] - a[1];
		double b2 = d[1] - b[1];
		double b3 = d[1] - c[1];
		
		double c1 = d[2] - a[2];
		double c2 = d[2] - b[2];
		double c3 = d[2] - c[2];

		double res = a1*b2*c3 - a1*b3*c2 + b1*c2*a3 - b1*c3*a2 + c1*a2*b3 - c1*a3*b2;

		return res;
	}

	point3_t cross(const point3_t& a, const point3_t& b) {
		point3_t res;
		res[0] = a[1] * b[2] - a[2] * b[1];
		res[1] = a[2] * b[2] - a[0] * b[2];
		res[2] = a[0] * b[1] - a[1] - b[0];
		return res;
	}

	bool edgesIntersection(const edge_t& e1, const edge_t& e2) {
		point3_t b1a1 = e1.b - e1.a;
		point3_t b2a2 = e2.b - e2.a;
		
		point3_t a2b1 = e2.a - e1.b;
		point3_t b2b1 = e2.b - e1.b;

		point3_t a1b2 = e1.a - e2.b;
		point3_t b1b2 = e1.b - e2.b;

		point3_t c1 = cross(b1a1, a2b1);
		point3_t c2 = cross(b1a1, b2b1);

		if (dot(c1, c2) > 0)
			return 0;

		point3_t c3 = cross(b2a2, a1b2);
		point3_t c4 = cross(b2a2, b1b2);

		if (dot(c3, c4) > 0)
			return 0;

		return 1;
	}

	int trianglesIntersection(face_t& face1, face_t& face2, double eps = -1e4) {
		if (face1.n_points() != 3 || face2.n_points() != 3)
			return -1;
		
		if (!face1.get_box().boundingBoxIntersect(face2.get_box()))
			return 0;
		
		double y1 = pointDeterminant(face1[0], face1[1], face1[2], face2[0]);
		double y2 = pointDeterminant(face1[0], face1[1], face1[2], face2[1]);
		double y3 = pointDeterminant(face1[0], face1[1], face1[2], face2[2]);

		if (y1 > 0 && y2 > 0 && y3 > 0 || y1 < 0 && y2 < 0 && y3 < 0)
			return 0;

		if (std::fabs(y1) < eps && std::fabs(y2) < eps && std::fabs(y3) < eps) {
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					if (edgesIntersection({face1[i], face1[(i + 1) % 3]}, {face2[j], face2[(j + 1) % 3]}))
						return 5;
			
			if (face2.pointInsideTrianle(face1[0]) || face1.pointInsideTrianle(face2[0]))
				return 5;

			return 0;
		}

		double y4 = pointDeterminant(face2[0], face2[1], face2[2], face1[0]);
		double y5 = pointDeterminant(face2[0], face2[1], face2[2], face1[1]);
		double y6 = pointDeterminant(face2[0], face2[1], face2[2], face1[2]);

		if (y4 > 0 && y5 > 0 && y6 > 0 || y4 < 0 && y5 < 0 && y6 < 0)
			return 0;

		if ((y5 < 0 && y4 > 0 && y6 > 0) || (y5 > 0 && y4 < 0 && y6 < 0)) {
			face1.rotateFaceNodes(2);
			double Y = pointDeterminant(face2[0], face2[1], face2[2],face1[0]);
			if (Y < 0) {
				face2.swapFaceNodes(1, 2);
			}
		}
		else if ((y6 < 0 && y4 > 0 && y5 > 0) || (y6 > 0 && y4 < 0 && y5 < 0)) {
			face1.rotateFaceNodes(1);
			double Y = pointDeterminant(face2[0], face2[1], face2[2], face1[0]);
			if (Y < 0) {
				face2.swapFaceNodes(1, 2);
			}
		}

		if ((y2 < 0 && y1 > 0 && y3 > 0) || (y2 > 0 && y1 < 0 && y3 < 0)){
			face2.rotateFaceNodes(2);
			double Y = pointDeterminant(face1[0], face1[1], face1[2], face2[0]);
			if (Y < 0) {
				face1.swapFaceNodes(1, 2);
			}
		}
		else if ((y3 < 0 && y1 > 0 && y2 > 0) || (y3 > 0 && y1 < 0 && y2 < 0)) {
			face2.rotateFaceNodes(1);
			double Y = pointDeterminant(face1[0], face1[1], face1[2], face2[0]);
			if (Y < 0) {
				face1.swapFaceNodes(1, 2);
			}
		}

		double y7 = pointDeterminant(face1[0], face1[1], face2[0], face2[1]);	
		double y8 = pointDeterminant(face1[0], face1[2], face2[2], face2[0]);	
		double y9 = pointDeterminant(face1[0], face1[2], face2[1], face2[0]);	
		double y10 = pointDeterminant(face1[0], face1[1], face2[2], face2[0]);	

		if (!(y7 <= 0 && y8 <= 0))
			return 0;
		
		if (y9 > 0 && y10 > 0)
			return 1;
		
		if (y9 > 0 && y10 <= 0)
			return 2;
		
		if (y9 <= 0 && y10 > 0)
			return 3;
		
		if (y9 <= 0 && y10 <= 0)
			return 4;

		return 0;
	}
}