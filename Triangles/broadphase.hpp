#pragma once

#include "triangles.hpp"

#include <algorithm>
#include <array>
#include <vector>
#include <set>
#include <queue>
#include <unordered_map>
#include <iostream>
#include <list>

namespace broadphase {
    class octree {
        public:
            using size_type = size_t;
        
        private:
            struct octree_node;

            size_type depth_;
            geometry::BoundingBox box_;
            std::vector<geometry::Triangle3> triangles;
            std::vector<octree_node> nodes;
            std::unordered_map<size_type, std::list<size_type>> mp;

        private:
            struct octree_node {
                geometry::BoundingBox box;
                std::vector<size_type> contained_triangles;
                std::vector<int> children;

                octree_node(const geometry::Vec3& a, const geometry::Vec3& b) : box(a, b) {
                    children.resize(8, -1);
                }
            };

        public:
            octree(size_type depth, const geometry::BoundingBox& box) : depth_{depth}, box_(box) {
                nodes.emplace_back(box_.get_min(), box.get_max());
            }

            template <typename It>
            octree(size_type depth, const geometry::BoundingBox& box, It begin, It end) : octree(depth, box) {
                
                while (begin != end) {
                    insert_face(*begin);
                    begin++;        
                }
            }

            void insert_face(const geometry::Triangle3& face) {
                triangles.push_back(face);
                insert_face_impl(triangles.size() - 1, 0, 0);
            }
            
            std::vector<size_type> get_potential_collision(size_type id) const {
                auto find = mp.find(id);
                std::vector<size_type> potential_collision;
                if (find == mp.end())
                    return potential_collision;
                std::list<size_type> lst = find->second;
                for (auto it : lst) {
                    octree_node node = nodes[it];
                    for (auto x : node.contained_triangles) {
                        if (x != id)
                            potential_collision.push_back(x);
                    }
                }
                return potential_collision;
            }

            void insert_face_impl(size_type id, int node_id, size_type depth) {
                auto face = triangles[id];
                auto aabb = face.box;

                if (!aabb.isInsideBoundingBox(nodes[node_id].box) && !aabb.boundingBoxIntersect(nodes[node_id].box) && !nodes[node_id].box.isInsideBoundingBox(aabb))
                    return;
                
                if (depth_ == depth) {
                    nodes[node_id].contained_triangles.push_back(id);
                    mp[id].push_back(node_id);
                }

                if (depth_ <= depth)
                    return;                

                for (int i = 0; i < 8; i++) {
                    if (nodes[node_id].children[i] == -1) {
                        geometry::Vec3 a = nodes[node_id].box.get_center();
                        geometry::Vec3 b;

                        geometry::Vec3 min_ = nodes[node_id].box.get_min(), max_ = nodes[node_id].box.get_max();

                        for (int j = 0; j < 3; j++)
                            b[j] = (i & (1 << j)) ? min_[j] : max_[j];

                        geometry::BoundingBox box(a, b);

                        if (!aabb.isInsideBoundingBox(box) && !aabb.boundingBoxIntersect(box)  && !nodes[node_id].box.isInsideBoundingBox(aabb))
                            continue;

                        nodes.emplace_back(a, b);
                        nodes[node_id].children[i] = nodes.size() - 1;
                    }
                    geometry::BoundingBox child_box = nodes[nodes[node_id].children[i]].box;

                    //if (aabb.isInsideBoundingBox(child_box) || aabb.boundingBoxIntersect(child_box))
                        insert_face_impl(id, nodes[node_id].children[i], depth + 1);
                }
            }

            void dump() const {
                for (int i = 0; i < nodes.size(); i++) {
                    if (nodes[i].contained_triangles.size() > 0) {
                        std::cout << i << " ";
                        std::cout << "box: min: ";
                        for (int j = 0; j < 3; j++)
                            std::cout << nodes[i].box.get_min()[j] << ' ';
                        std::cout << "max: ";
                        for (int j = 0; j < 3; j++)
                            std::cout << nodes[i].box.get_max()[j] << ' ';
                        std::cout << "\ncontained triangles: " << nodes[i].contained_triangles.size() << "\n";
                        // for (int j = 0; j < 3; j++)
                        //     std::cout << nodes[nodes[i].contained_triangles[0]].box.get_min()[j] << ' ';
                        // for (int j = 0; j < 3; j++)
                        //     std::cout << nodes[nodes[i].contained_triangles[0]].box.get_max()[j] << ' ';
                        // std::cout << "\n\n";
                        
                    }
                    // for (auto x : nodes[i].contained_triangles) {
                    //     for (int k = 0; k < 3; k++) {
                    //         for (int m = 0; m < 3; m++) {
                    //             std::cout << triangles[x][k][m] << " ";
                    //         }
                    //         std::cout << "\n";
                    //     }
                    // }
                }
            }
    };
}