#pragma once

#include <array>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <iostream>

namespace geometry {
	
	bool is_roughly_equal(double x, double y, double eps = 1e-6) {
		return std::abs(x - y) < eps;
	}

	struct Vec2 {
		double x, y;
		bool operator==(const Vec2& rhs) const {
			return  is_roughly_equal(x, rhs.x) &&
					is_roughly_equal(y, rhs.y);
		}

		bool operator!=(const Vec2& rhs) const {
			return !(*this == rhs);
		}
		
		Vec2& operator*=(double k) & {
			x *= k;
			y *= k;
			return *this;
		}

		Vec2 operator*(double k) const {
			Vec2 tmp(*this);
			tmp *= k;
			return tmp; 
		}
		
		Vec2& operator/=(double k) & {
			x /= k;
			y /= k;
			return *this;
		}

		Vec2 operator/(double k) const {
			Vec2 tmp(*this);
			tmp /= k;
			return tmp; 
		}

		Vec2 operator-() const {
			Vec2 tmp(*this);
			tmp *= -1;
			return tmp;
		}
		
		void dump() const {
			std::cout << "x: " << x << " y: " << y << '\n';
		}

		static Vec2 zero() { return {0, 0}; }

		double dot(const Vec2& other) const {
			return x * other.x + y * other.y;
		}

		Vec2 perpendicular() const {
			return {-y, x};
		}

		double sq_length() const {
			return dot(*this);
		}

		double length() const {
			return std::sqrt(sq_length());
		}

		Vec2& normalize() {
			double len = length();
			if (is_roughly_equal(len, 0))
				throw std::runtime_error("trying to normalize zero vector");
			*this /= len;
			return *this;
		}

		Vec2 normalized() const {
			Vec2 tmp(*this);
			tmp.normalize();
			return tmp;
		}

		Vec2 project(const Vec2& other) const {
			double other_sq_len = other.sq_length();
			if (is_roughly_equal(other_sq_len, 0))
				throw std::runtime_error("trying to project on a zero vector");
			return (other * dot(other)) / other_sq_len;
		}
	};

	double dot(const Vec2& v1, const Vec2& v2) { return v1.dot(v2); }
	Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
		return {lhs.x + rhs.x, lhs.y + rhs.y};
	}

	Vec2 operator-(const Vec2& lhs, const Vec2& rhs) {
		return lhs + (-rhs);
	}

	struct Vec3 {
		double x, y, z;

		double& operator[](int id) & {
			switch (id) {
				case 0: return x;
				case 1: return y;
				case 2: return z;
				default: throw std::runtime_error("invalid index in Vec3 op[]");
			}
		}

		const double& operator[](int id) const & {
			switch (id) {
				case 0: return x;
				case 1: return y;
				case 2: return z;
				default: throw std::runtime_error("invalid index in Vec3 op[]");
			}
		}

		bool operator==(const Vec3& rhs) const {
			return  is_roughly_equal(x, rhs.x) &&
					is_roughly_equal(y, rhs.y) &&
					is_roughly_equal(z, rhs.z);
		}
		
		bool operator!=(const Vec3& rhs) const {
			return !(*this == rhs);
		}

		Vec3& operator*=(double k) & {
			x *= k;
			y *= k;
			z *= k;

			return *this;
		}

		Vec3 operator*(double k) const {
			Vec3 tmp(*this);
			tmp *= k;
			return tmp; 
		}

		Vec3 operator-() const {
			Vec3 tmp(*this);
			tmp *= -1;
			return tmp;
		}

		Vec3& operator/=(double k) & {
			x /= k;
			y /= k;
			z /= k;

			return *this;
		}

		Vec3 operator/(double k) const {
			Vec3 tmp(*this);
			tmp /= k;
			return tmp; 
		}

		void dump() const {
			std::cout << "x: " << x << " y: " << y << " z: " << z << '\n';
		}

		static Vec3 zero() { return {0, 0, 0}; }

		double dot(const Vec3& other) const {
			return x * other.x + y * other.y + z * other.z;
		}

		Vec3 cross(const Vec3& other) const {
			return {y * other.z - z * other.y, 
					z * other.x - x * other.z, 
					x * other.y - y * other.x };
		}

		double sq_length() const {
			return dot(*this);
		}

		double length() const {
			return std::sqrt(sq_length());
		}

		Vec3& normalize() {
			double len = length();
			if (is_roughly_equal(len, 0))
				throw std::runtime_error("trying to normalize zero vector");
			*this /= len;
			return *this;
		}

		Vec3 normalized() const {
			Vec3 tmp(*this);
			tmp.normalize();
			return tmp;
		}
		
		/*projects *this on other*/
		Vec3 project(const Vec3& other) const {
			double other_sq_len = other.sq_length();
			if (is_roughly_equal(other_sq_len, 0))
				throw std::runtime_error("trying to project on a zero vector");
			
			return (other * dot(other)) / other_sq_len;
		}

		/*projects vec on axis*/
		Vec2 projectOnAxis(int axis) const {
			switch (axis) {
			case 0: return Vec2{y, z};
			case 1: return Vec2{x, z};
			case 2: return Vec2{x, y};
			default: throw std::runtime_error("invalid axis index in projectOnAxis");
			}
		}
	};

	Vec3 operator*(double k, const Vec3& rhs) {
		return rhs * k;
	}

	Vec3 operator+(const Vec3& lhs, const Vec3& rhs) {
		return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
	}

	Vec3 operator-(const Vec3& lhs, const Vec3& rhs) {
		return lhs + (-rhs);
	}

	double dot(const Vec3& v1, const Vec3& v2) {
		return v1.dot(v2);
	}

	Vec3 cross(const Vec3& v1, const Vec3& v2) {
		return v1.cross(v2);
	} 

	struct BoundingBox {
		Vec3 min, max;
		
		BoundingBox(const Vec3& a, const Vec3& b) : 
			min{std::min(a[0], b[0]), std::min(a[1], b[1]), std::min(a[2], b[2])}, 
			max{std::max(a[0], b[0]), std::max(a[1], b[1]), std::max(a[2], b[2])} {}
		
		BoundingBox(const Vec3& a, const Vec3& b, const Vec3& c) {
			for (int i = 0; i < 3; ++i) {
				min[i] = std::min(a[i], std::min(b[i], c[i]));
				max[i] = std::max(a[i], std::max(b[i], c[i]));
			}
		}

		bool isInsideBoundingBox(const BoundingBox& other) const {
			return ((max[0] < other.max[0]) &&
					(max[1] < other.max[1]) &&
					(max[2] < other.max[2]) &&
					(min[0] > other.min[0]) &&
					(min[1] > other.min[1]) &&
					(min[2] > other.min[2]));
		}

		bool boundingBoxIntersect(const BoundingBox& other) const {
			return !((min[0] > other.max[0]) ||
					(min[1] > other.max[1]) ||
					(min[2] > other.max[2]) ||
					(max[0] < other.min[0]) ||
					(max[1] < other.min[1]) ||
					(max[2] < other.min[2]));
		}

		bool intersect(const BoundingBox& other) const {
			if (max == other.max || max == other.min || min == other.max || min == other.min)
				return true;
			
			if (boundingBoxIntersect(other))
				return true;
			
			return (isInsideBoundingBox(other) || other.isInsideBoundingBox(*this));
		}

		Vec3 get_min() const { return min; }
		Vec3 get_max() const { return max; }
		Vec3 get_center() const { return (min + max) / 2; }
	};
	struct Line3 {
		Vec3 direction;
		Vec3 origin;

		// Line3() = default;
		// Line3(const Vec3& dir, const Vec3& ori) : direction{dir}, origin{ori} {}
		// Line3(const Vec3& a, const Vec3& b) : direction{a - b}, origin{a} {}

		bool isPointOnLine(const Vec3& p) {
			Vec3 v1 = p - origin;
			Vec3 v2 = direction;

			return cross(v1, v2) == Vec3::zero();
		}
	};

	struct LineSegment3 {
		Vec3 a, b;
		
		bool intersect(const LineSegment3& other) const {
			double w1 = dot(cross(a - b, b - other.b), cross(a - b, b - other.a));
			double w2 = dot(cross(other.a - other.b, other.b - b), cross(other.a - other.b, other.b - a));
			return (w1 <= 0 && w2 <= 0);
		} 
	};

	struct LineSegment2 {
		Vec2 a, b;

		bool intersect(const LineSegment2& other) const {
			Vec3 a3 = {a.x, a.y, 0}, b3 = {b.x, b.y, 0};
			Vec3 oa3 = {other.a.x , other.a.y, 0}, ob3 = {other.b.x, other.b.y, 0};
			LineSegment3 a1 = {a3, b3}, b1 = {oa3, ob3};
			
			return a1.intersect(b1);
		}
	};

	struct LineSegment1 {
	private:
		double min, max;
	public:
		LineSegment1(double a_, double b_) : min{std::min(a_, b_)}, max{std::max(a_, b_)} {}
		bool intersect(const LineSegment1& other) {
			if (is_roughly_equal(min, other.max) || is_roughly_equal(max, other.min))
				return true;
			
			return !(max < other.min || other.max < min);
		}
	};
	struct Plane {
	private:
		Vec3 normal_;
		double dist_;
	public:
		Plane (const Vec3& n, double d) : normal_(n.normalized()), dist_(d) {}
		Plane(const Vec3& n, const Vec3& point) : normal_(n.normalized()), dist_(dot(point, normal_)) {}
		Plane(const Vec3& p1, const Vec3& p2, const Vec3& p3) try : Plane(cross(p1 - p2, p1 - p3).normalized(), p1) {}
		catch(const std::runtime_error& e) {
			throw std::runtime_error("Impossible to create plane from these points");
		}
		
		bool parallel(const Plane& other) {
			return (normal_ == other.normal_ || -normal_ == other.normal_);
		}

		bool operator==(const Plane& other) {
			return parallel(other) && is_roughly_equal(dist_, other.dist_);
		}

		bool operator!=(const Plane& other) { return !(*this == other); }

		double signedDistance(const Vec3& p) const { return dot(p, normal_) - dist_; }
		double distance(const Vec3& p) const { return std::abs(signedDistance(p)); }

		Line3 intersection(const Plane& other) const {
			Line3 line;
			line.direction = cross(normal_, other.normal_);
			if (line.direction == Vec3::zero())
				throw std::runtime_error("Trying to compute intersection of two parallel planes");
			
			double s1 = dist_, s2 = other.dist_;
			double n1n2dot = dot(normal_, other.normal_);
			double n1lensq = normal_.sq_length();
			double n2lensq = other.normal_.sq_length();
			double a = (s2 * n1n2dot - s1 * n1lensq) / (n1n2dot * n1n2dot - n1lensq * n2lensq);
			double b = (s1 * n1n2dot - s2 * n2lensq) / (n1n2dot * n1n2dot - n1lensq * n2lensq);
			line.origin = a * normal_ + b * other.normal_;
			return line;
		}

		double dist() const { return dist_; }
		Vec3 normal() const { return normal_; }
		void dump() const {
			normal_.dump();
			std::cout << "d: " << dist_ << '\n';
		}
	};

	double signedDistance(const Plane& p, const Vec3& v) {
		return p.signedDistance(v);
	}
	
	struct Triangle2 {
		Vec2 a, b, c;

		bool isPointInsideTriangle(const Vec2& p) const {
			double w1 = (a.x * (c.y - a.y) + (p.y - a.y) * (c.x - a.x) - p.x * (c.y - a.y)) /
						((b.y - a.y) * (c.x - a.x) - (b.x - a.x) * (c.y - a.y));
			double w2 = (p.y - a.y - w1 * (b.y - a.y)) / (c.y - a.y);

			return (w1 >= 0 && w2 >= 0 && (w1 + w2) <= 1);
		}

		bool intersect(const Triangle2& other) const {
			std::vector<LineSegment2> edges = {{a, b}, {b, c}, {c, a}};
			std::vector<LineSegment2> o_edges = {{other.a, other.b}, {other.c, other.c}, {other.c, other.a}};
			
			for (const auto& edge1 : edges)
				for (const auto& edge2 : o_edges)
					if (edge1.intersect(edge2))
						return true;
			
			return (isPointInsideTriangle(other.a) || other.isPointInsideTriangle(a));
		}
	};

	struct Triangle3 {
		Vec3 a, b, c;
		BoundingBox box;

		Triangle3(const Vec3& a_, const Vec3& b_, const Vec3& c_) : a{a_}, b{b_}, c{c_}, box(a, b, c) {}
		
		bool isDegeneratePoint() const {
			return a == b && b == c;
		}

		bool isDegenerateLine() const {
			return cross(a - b, a - c) == Vec3::zero();
		}

		Plane plane() const { return Plane(a, b, c); }
		Triangle2 projectOnAxis(int axis) const {
			return Triangle2{a.projectOnAxis(axis), b.projectOnAxis(axis), c.projectOnAxis(axis)};
		}
		
		Triangle2 projectOnSomeAxis() const {
			int axis = 0;
			if (plane().normal().dot({0, 0, 1}) == 0)
				axis = 1;
			return projectOnAxis(axis);
		}

		std::pair<Triangle3, std::vector<double>> rotateTriangleEdges(std::vector<double> dist) const {
			auto cnt = std::count_if(dist.begin(), dist.end(), [](double d) { return d > 0; });
			switch (cnt) {
				case 1:
					break;
				case 2:
					std::for_each(dist.begin(), dist.end(), [](double& d) { d *= -1; });
					break;
				default:
					throw std::runtime_error("distances are all positive");
			}

			auto max_id = std::distance(dist.begin(), std::max_element(dist.begin(), dist.end()));
			switch (max_id) {
				case 0:
					return {Triangle3{b, a, c}, std::vector<double>{dist[1], dist[0], dist[2]}};
				case 1:
					return {Triangle3{a, b, c}, std::vector<double>{dist[0], dist[1], dist[2]}};
				case 2:
					return {Triangle3{a, c, b}, std::vector<double>{dist[0], dist[2], dist[1]}};
				default:
					throw std::runtime_error("Invalid distances array passed into MakeCanonicalTriangle");
			}
		}
		
		int handleDegenerateCases(const Triangle3& other) const {
			if (isDegeneratePoint()) {
				if (!other.isDegenerateLine()) {
					Triangle2 t2 = other.projectOnAxis(0);
					Vec2 p = a.projectOnAxis(0);
					return t2.isPointInsideTriangle(p);
				}
				
				if (other.isDegeneratePoint())
					return a == other.a;
				
				Line3 line = (other.a == other.b ? Line3{other.a - other.c, other.a} : Line3{other.a - other.b, other.a});
				return line.isPointOnLine(a);
			}

			if (other.isDegeneratePoint()) {
				if (!isDegenerateLine()) {
					Triangle2 t1 = projectOnAxis(0);
					Vec2 p = a.projectOnAxis(0);
					return t1.isPointInsideTriangle(p);
				}
				
				if (isDegeneratePoint())
					return a == other.a;
				
				Line3 line = (a == b ? Line3{a - c, a} : Line3{a - b, a});
				return line.isPointOnLine(other.a);
			}

			if (isDegenerateLine() || other.isDegenerateLine()) {
				Triangle2 t1 = projectOnSomeAxis();
				Triangle2 t2 = other.projectOnSomeAxis();
				return t1.intersect(t2);
			}

			return -1;
		}

		bool intersect(const Triangle3& other) const {
			if (!box.intersect(other.box))
				return false;
			
			int res = 0;
			if ((res = handleDegenerateCases(other)) != -1)
				return res;

			Plane plane1 = plane();
			std::vector<double> dist1{plane1.signedDistance(other.a),
									plane1.signedDistance(other.b),
									plane1.signedDistance(other.c)};
			std::for_each(dist1.begin(), dist1.end(), [](double& x) { x = is_roughly_equal(x, 0) ? 0 : x; });
			if ((dist1[0] > 0 && dist1[1] > 0 && dist1[2] > 0) || (dist1[0] < 0 && dist1[1] < 0 && dist1[2] < 0))
				return false;
			
			Plane plane2 = other.plane();

			if (plane1.parallel(plane2)) {
				if (plane1.dist() != plane2.dist()) {
					return false;
				}
				
				Triangle2 t1 = projectOnSomeAxis();
				Triangle2 t2 = other.projectOnSomeAxis();
				return t1.intersect(t2);
			}

			std::vector<double> dist2{plane2.signedDistance(a),
									plane2.signedDistance(b),
									plane2.signedDistance(c)};
			std::for_each(dist2.begin(), dist2.end(), [](double& x) { x = is_roughly_equal(x, 0) ? 0 : x; });
			if ((dist2[0] > 0 && dist2[1] > 0 && dist2[2] > 0) || (dist2[0] < 0 && dist2[1] < 0 && dist2[2] < 0))
				return false;
			
			Line3 line = plane1.intersection(plane2);
			
			std::pair<Triangle3, std::vector<double>> canon1 = rotateTriangleEdges(dist2);
			std::pair<Triangle3, std::vector<double>> canon2 = other.rotateTriangleEdges(dist1);
			
			std::vector<double> values{line.direction[0], line.direction[1], line.direction[2]};
			auto max_id = std::distance(values.begin(), std::max_element(values.begin(), values.end()));

			std::vector<double> t1{canon1.first.a[max_id], canon1.first.b[max_id], canon1.first.c[max_id]};
			std::vector<double> t2{canon2.first.a[max_id], canon2.first.b[max_id], canon2.first.c[max_id]};

			double t00 = t1[0] + (t1[1] - t1[0]) * (canon1.second[0]) / (canon1.second[0] - canon1.second[1]);
			double t01 = t1[2] + (t1[1] - t1[2]) * (canon1.second[2]) / (canon1.second[2] - canon1.second[1]);

			double t10 = t2[0] + (t2[1] - t2[0]) * (canon2.second[0]) / (canon2.second[0] - canon2.second[1]);
			double t11 = t2[2] + (t2[1] - t2[2]) * (canon2.second[2]) / (canon2.second[2] - canon2.second[1]);

			LineSegment1 seg1(t00, t01), seg2(t10, t11);

			return seg1.intersect(seg2);
		}
	};
}