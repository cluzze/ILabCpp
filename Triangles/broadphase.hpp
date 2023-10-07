#pragma once

#include "triangles.hpp"

#include <algorithm>
#include <array>
#include <vector>
#include <set>

namespace broadphase {
    class octree {
        public:
            using size_type = size_t;
        
        private:
            struct octree_node;

            std::vector<geometry::face_t> triangles;
            std::vector<octree_node> nodes;

        private:
            struct octree_node {
                geometry::bounding_box_t box;
                std::vector<size_type> contained_triangles;
                std::vector<size_type> children;

                octree_node(const geometry::point3_t& a, const geometry::point3_t& b) 
                        : box(a, b), children(8, -1) {}
            };

        public:
            octree() {}

            template <typename It>
            octree(It begin, It end) {

            }

            void insert_face(const geometry::face_t& face) {
                triangles.push_back(face);
                insert_face_impl(triangles.size() - 1, nodes[0]);
            }

            void insert_face_impl(size_type id, octree_node& node) {
                auto face = triangles[id];
                auto aabb = face.get_box();

                if (!aabb.isInsideBoundingBox(node.box))
                    return;
                
                node.contained_triangles.push_back(id);

                for (int i = 0; i < 8; i++) {
                    if (node.children[i] == -1) {
                        geometry::point3_t a = node.box.get_center();
                        geometry::point3_t b;

                        geometry::point3_t min_ = node.box.get_min(), max_ = node.box.get_max();

                        for (int j = 0; j < 3; j++)
                            b[j] = (i & (j + 1)) ? min_[j] : max_[j];

                        nodes.emplace_back(a, b);
                        node.children[i] = nodes.size() - 1;
                    }

                    if (aabb.boundingBoxIntersect(node.box))
                        insert_face_impl(id, nodes[node.children[i]]);
                }
            }
    };
}