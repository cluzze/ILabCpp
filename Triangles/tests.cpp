#include <gtest/gtest.h>

#include "triangles.hpp"
#include "broadphase.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <random>
#include <iostream>
#include <fstream>

using namespace geometry;

TEST(Vec3, dot) {
	Vec3 v1{1, 1, 3}, v2{3, 2, 1}, v3{1, 0, 4};
	double d12 = v1.dot(v2);
	double d13 = v1.dot(v3);
	double d23 = v2.dot(v3);
	ASSERT_EQ(d12, 8);
	ASSERT_EQ(d13, 13);
	ASSERT_EQ(d23, 7);
}

TEST(Vec3, cross) {
	Vec3 v1 = {1, 2, 3}, v2 = {3, 2, 1}, v3 = {5, 2, 3};
	Vec3 exp1 = {-4, 8, -4}, exp2 = {0, 12, -8}, exp3 = {4, -4, -4};
	bool res1 = v1.cross(v2) == exp1;
	bool res2 = v1.cross(v3) == exp2;
	bool res3 = v2.cross(v3) == exp3;
	ASSERT_TRUE(res1);
	ASSERT_TRUE(res2);
	ASSERT_TRUE(res3);
	ASSERT_TRUE(v2.cross(v1) == -exp1);
	ASSERT_TRUE(v3.cross(v1) == -exp2);
	ASSERT_TRUE(v3.cross(v2) == -exp3);
}

TEST(Vec3, length) {
	Vec3 v1 = {1, 2, 3}, v2 = {0, 0, 0};
	double len1 = v1.length(), len2 = v2.length();
	double exp1 = std::sqrt(14), exp2 = 0;
	bool res1 = is_roughly_equal(len1, exp1);
	bool res2 = is_roughly_equal(len2, exp2);
	ASSERT_TRUE(res1);
	ASSERT_TRUE(res2);
}

TEST(Vec3, normalize) {
	Vec3 v1 = {1, 2, 3};
	double expLen1 = std::sqrt(14);
	Vec3 exp1 = {1 / expLen1, 2 / expLen1, 3 / expLen1};
	v1.normalize();
	bool res1 = v1 == exp1;
	ASSERT_TRUE(res1);
	Vec3 old_v1 = v1;
	v1.normalize();
	ASSERT_TRUE(old_v1 == v1);

	Vec3 vv = v1.normalize();
}

TEST(Vec3, project) {
	Vec3 v1{1, 2, 3}, v2{3, 2, 1};
	Vec3 v3 = v1.project(v2);
	Vec3 exp1{ (double)15 / 7, (double)10/7, (double)5/7};
	bool res1 = exp1 == v3;
	ASSERT_TRUE(res1);
	
	Vec3 v4{1, 0, 3}, v5{-1, 4, 2};
	Vec3 v6 = v5.project(v4);
	Vec3 exp2{ (double)1 / 2, 0, (double)3/2};
	bool res2 = exp2 == v6;
	ASSERT_TRUE(res2);
}

TEST(Vec3, op) {
	Vec3 v1{1, 2, 3}, v2{3, 2, 1}, v3{4, 4, 4};
	Vec3 v4 = v3 - v2;
	ASSERT_TRUE((v3 - v2) == v1);
	ASSERT_TRUE((v1 + v2) == v3);
}

TEST(Plane, create1) {
	Vec3 v1{0.5, 3, 4}, v2{1, -2, 1}, v3{-2, 3, 2};
	Plane p(v1, v2, v3);
	//p.dump();
	//Vec3
}

TEST(Plane, create2) {
	Vec3 v1{1, 2, 3}, v2{3, 2, 1}, v3{1, 1, 1};
	Plane p(v1, v2, v3);
	double expd = 0;
	Vec3 exp = {-1, 2, -1};
	ASSERT_TRUE(is_roughly_equal(p.dist(), expd));
	ASSERT_TRUE(exp.normalized() == p.normal());
}

TEST(Plane, create3) {
	Vec3 v1{1, 1, 1}, v2{2, 2, 2}, v3{3, 3, 3};
	try {
		Plane p(v1, v2, v3);
	} catch (const std::runtime_error& e) {
		//std::cout << e.what();
	}
}

TEST(Plane, create4) {
	Vec3 v1{1, 2, 3}, v2{3, 2, 1}, v3{3, 1, 2};
	Vec3 n1{1, 2, 1}, n2{5, 3, 7}, n3{4, 2, 1};
	Plane p1(n1, v1), p2(n2, v2), p3(n3, v3);
	Plane exp1(n1.normalized(), -signedDistance(p1, Vec3::zero())), exp2(n2.normalized(), 28 / n2.length()), exp3(n3.normalized(), 16 / n3.length());
	//p1.dump();
	//exp1.dump();
	ASSERT_TRUE(p1 == exp1);
	ASSERT_TRUE(p2 == exp2);
	ASSERT_TRUE(p3 == exp3);
}

TEST(Triangle2, point_in_triangle) {
	std::vector<Vec2> inside{{0.75, 0.75}, {0.99, 0.99}, {0.75, 1}, {1, 0.75}, {0.5, 0.5}};;
	Vec2 a{1, 0}, b{1, 1}, c{0, 1};
	Triangle2 t1{a, b, c};
	for (auto p : inside) {
		ASSERT_TRUE(t1.isPointInsideTriangle(p));
	}
	ASSERT_TRUE(t1.isPointInsideTriangle(a));
	ASSERT_TRUE(t1.isPointInsideTriangle(b));
	ASSERT_TRUE(t1.isPointInsideTriangle(c));

	std::vector<Vec2> outside{{0, 0}, {0.49, 0.49}, {0, 0.99}, {1.5, 1.5}};
	for (auto p : outside) {
		ASSERT_FALSE(t1.isPointInsideTriangle(p));
	}
}

TEST(LineSegment, intersect) {
	LineSegment2 l1 = {{0, 0}, {1, 1}}, l2 = {{1, 0}, {0, 1}};
	ASSERT_TRUE(l1.intersect(l2));
	LineSegment2 l3 = {{0, 0}, {1, 1}}, l4 = {{2, 0}, {0, 1}};
	ASSERT_TRUE(l3.intersect(l4));
	LineSegment2 l5 = {{0, 0}, {0.5, 0.5}}, l6 = {{1, 0}, {0, 1}};
	ASSERT_TRUE(l5.intersect(l6));
}

TEST(Plane, intersection1) {
	Plane p1{{0, 0, 1}, 0}, p2{{1, 0, 0}, 1};
	Line3 l = p1.intersection(p2);
	l.direction.dump();
	l.origin.dump();
	ASSERT_TRUE(l.isPointOnLine({1, 0, 0}));
	ASSERT_TRUE(l.isPointOnLine({1, 10, 0}));
}

TEST(Plane, intersection2) {
	Plane p1{{0, 0, 1}, 0}, p2{{1, 1, 0}, 1};
	Line3 l = p1.intersection(p2);
	l.direction.dump();
	l.origin.dump();
	ASSERT_TRUE(l.isPointOnLine({1 / std::sqrt(2), 1 / std::sqrt(2), 0}));
	ASSERT_TRUE(l.isPointOnLine({0, std::sqrt(2), 0}));
	ASSERT_TRUE(l.isPointOnLine({std::sqrt(2), 0, 0}));
}

TEST(Triangle3, Intersect1) {
	Triangle3 a{{0, 0, 0}, {1, 1, 1}, {3, 5, 2}};
	Triangle3 b{{3, 4, 1}, {2, 1, 4}, {-4, 3, 0}};
	ASSERT_TRUE(a.intersect(b));
}

TEST(Triangle3, Intersect2) {
	Triangle3 a{{0, 0, 0}, {1, 1, 1}, {1, 2, 3}};
	Triangle3 b{{3, 2, 1}, {2, 1, 4}, {-4, 3, 0}};
	ASSERT_TRUE(a.intersect(b));
}

TEST(Triangle3, Intersect3) {
	Triangle3 a{{0, 0, 0}, {1, 1, 0}, {1, 2, 0}};
	Triangle3 b{{3, 2, 1}, {2, 1, 1}, {-4, 3, 0}};
	ASSERT_FALSE(a.intersect(b));
}

TEST(Triangle3, Intersect4) {
	Triangle3 a{{0, 0, 0}, {1, 1, 0}, {1, 2, 0}};
	Triangle3 b{{3, 2, 1}, {2, 1, 1}, {-4, 3, 1}};
	ASSERT_FALSE(a.intersect(b));
}

TEST(Triangle3, Intersect5) {
	Triangle3 a{{0, 0, 0}, {1, 1, 0}, {1, 2, 0}};
	Triangle3 b{{-1, 4, 5}, {1, 4, 1}, {0, 4, 0}};
	ASSERT_FALSE(a.intersect(b));
}

TEST(Triangle3, Intersect6) {
	Triangle3 a{{-3, 1, 0}, {1, 1, 0}, {1, -5, 0}};
	Triangle3 b{{-1, 0, 0}, {0, -1, 0}, {0, -0.5, 0}};
	ASSERT_TRUE(a.intersect(b));
}

TEST(Triangle3, Intersect7) {
	Triangle3 a{{-3, 1, 0}, {1, 1, 0}, {1, -5, 0}};
	Triangle3 b{{1, -6, 0}, {1, 2, 1}, {1, 5, -4}};
	ASSERT_TRUE(a.intersect(b));
}

TEST(Triangle3, Intersect8) {
	Triangle3 a{{-3, 1, 0}, {1, 1, 0}, {1, -5, 0}};
	Triangle3 b{{1, -5, 0}, {3, 2, 1}, {1, 5, -4}};
	ASSERT_TRUE(a.intersect(b));
}

TEST(Triangle3, Intersect9) {
	Triangle3 a{{5, 0, 0}, {0, 5, 0}, {0, 0, 0}};
	Triangle3 b{{0, 0, 0}, {0, 5, 0}, {0, 0, 5}};
	ASSERT_TRUE(a.intersect(b));
}

TEST(Triangle3, Intersect10) {
	Triangle3 a{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
	Triangle3 b{{0, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	ASSERT_TRUE(a.intersect(b));
}

TEST(Triangle3, Intersect11) {
	Triangle3 a{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
	Triangle3 b{{0, 0, 0}, {0, 0, 0}, {0, 0, 1}};
	ASSERT_TRUE(a.intersect(b));
}

TEST(Triangle3, Intersect12) {
	Triangle3 a{{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
	Triangle3 b{{0, 0, 0}, {0, 0, 0}, {0, 0, 1}};
	ASSERT_FALSE(a.intersect(b));
}

TEST(Triangle3, Intersect13) {
	Triangle3 a{{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
	Triangle3 b{{0, 0, 0}, {1, 0, 0}, {0, 1, 0}};
	ASSERT_FALSE(a.intersect(b));
}

TEST(Triangle3, Intersect14) {
	Triangle3 a{{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
	Triangle3 b{{0, 0, 0}, {1, 0, 0}, {0, 1, 0}};
	ASSERT_FALSE(a.intersect(b));
}

TEST(Triangle3, Intersect15) {
	Triangle3 a{{0, 0, 0}, {1, 0, 0}, {0, 1, 0}};
	Triangle3 b{{0, 0, 0.01}, {5, 5, 0.01}, {5, 5, 0}};
	ASSERT_FALSE(a.intersect(b));
}

TEST(Triangles, e2e) {
	int n;
	std::ifstream file("tests/data.txt");
	file >> n;
	std::vector<Triangle3> tri;
	Vec3 min{1e9, 1e9, 1e9}, max{-1e9, -1e9, -1e9};
	for (int i = 0; i < n; i++) {
		std::vector<Vec3> v(3);
		for (int j = 0; j < 3; j++) {
			file >> v[j][0] >> v[j][1] >> v[j][2];
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
	file.close();
	geometry::BoundingBox box(min, max);
	broadphase::octree tree(5, box, tri.begin(), tri.end());

	std::set<int> collisions_naive, collisions_smart;

	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			int res = tri[i].intersect(tri[j]);
			if (res > 0) {			
				collisions_naive.insert(i);
				collisions_naive.insert(j);
			}
		}
	}

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
	ASSERT_TRUE(collisions_naive == collisions_smart);
}

TEST(Plane, seg_intersection1) {
	Plane p({0, 0, 1}, 0);
	LineSegment3 s1{{-1, -1, -1}, {1, 1, 1}};
	auto res = p.intersection(s1);
	ASSERT_TRUE(res.has_value());
	ASSERT_TRUE(res.value() == Vec3(0, 0, 0));
}

TEST(Plane, seg_intersection2) {
	Plane p({0, 0, 1}, 0);
	LineSegment3 s1{{0, 0, -1}, {1, 1, 1}};
	auto res = p.intersection(s1);
	ASSERT_TRUE(res.has_value());
	ASSERT_EQ(res.value(), Vec3(0.5, 0.5, 0));
}