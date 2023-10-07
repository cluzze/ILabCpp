#include <gtest/gtest.h>

#include "triangles.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <random>
#include <iostream>
#include <fstream>

using namespace geometry;

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

TEST(tri, triangle_inside_triangle_intersection) {
    face_t t1{{0, 0, 0},
            {1, 0, 0},
            {0, 1, 0}};

    face_t t2{{0, 0, 0},
            {0.5, 0, 0},
            {0, 0.5, 0}};
    
    int res = trianglesIntersection(t1, t2);
    ASSERT_TRUE(res);
}

TEST(tri, line_intersection) {
    face_t t1{{0, 0, 0},
            {1, 0, 0},
            {0, 1, 0}};

    face_t t2{{1, 1, 0},
            {1, 0, 0},
            {0, 1, 0}};
    
    int res = trianglesIntersection(t1, t2);
    ASSERT_TRUE(res);
}

TEST(tri, edge_intersection) {
    face_t t1{{0, 0, 0},
            {1, 0, 0},
            {0, 1, 0}};

    face_t t2{{1, 1, 1},
            {1, 0, 0},
            {0, 1, 0}};
    
    int res = trianglesIntersection(t1, t2);
    ASSERT_TRUE(res);
}

TEST(tri, part_edge_intersection) {
    face_t t1{{0, 0, 0},
            {1, 0, 0},
            {0, 1, 0}};

    face_t t2{{1, 1, 1},
            {0.66, 0, 0},
            {0, 1, 0}};
    
    int res = trianglesIntersection(t1, t2);
    ASSERT_TRUE(res);
}

TEST(tri, small_part_edge_intersection) {
    face_t t1{{0, 0, 0},
            {1, 0, 0},
            {0, 1, 0}};

    face_t t2{{1, 1, 1},
            {0.01, 0, 0},
            {0, 1, 0}};
    
    int res = trianglesIntersection(t1, t2);
    ASSERT_TRUE(res);
}

TEST(tri, 3dline_intersection) {
    face_t t1{{0, 0, 0},
            {1, 0, 0},
            {0, 1, 0}};

    face_t t2{{1, 1, 1},
            {0, 0, -1},
            {1, 1, -1}};
    
    int res = trianglesIntersection(t1, t2);
    ASSERT_TRUE(res);
}

TEST(tri, single_point_inside_face_intersection) {
    face_t t1{{0, 0, 0},
            {2, 0, 0},
            {0, 2, 0}};

    face_t t2{{1, 1, 1},
            {1, 1, -0.001},
            {0, 0, 1}};
    
    int res = trianglesIntersection(t1, t2);
    ASSERT_TRUE(res);
}

TEST(tri, single_point_intersection) {
    face_t t1{{0, 0, 0},
            {1, 0, 0},
            {0, 1, 0}};

    face_t t2{{0, 0, 0},
            {0, -1, 0},
            {0, 0, 1}};
    
    int res = trianglesIntersection(t1, t2);
    ASSERT_TRUE(res);
}