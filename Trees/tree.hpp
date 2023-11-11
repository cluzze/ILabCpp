#pragma once

#include <functional>
#include <iostream>
#include <list>
#include <stack>
#include <type_traits>

namespace containers
{
    template <typename KeyT, typename Comp = std::less<KeyT>>
	class SearchTree final {
    private:
        [[no_unique_address]] Comp cmp;

        enum class Color {
            Black = 0,
            Red = 1
        };

        struct TreeNode;
        using ListIt = std::list<TreeNode>::iterator;
        using CListIt = std::list<TreeNode>::const_iterator;
        struct TreeNode {
            KeyT key;
            int subtreeSize;
            Color color;
            ListIt p, left, right;
        };

        ListIt nil_;
        ListIt root_;

        std::list<TreeNode> nodes;
    public:
        class iterator {
        private:
            ListIt ptr;
        
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = ListIt::difference_type;
            using value_type = KeyT;
            using reference = const value_type&;
            using pointer = const value_type*;

        public:
            iterator(ListIt it = ListIt{}) : ptr(it) {}

            iterator& operator++() {
                ptr++;
                return *this;
            }

            iterator operator++(int) {
                iterator tmp(ptr);
                ptr++;
                return *this;
            }

            iterator& operator--() {
                ptr--;
                return *this;
            }

            iterator operator--(int) {
                iterator tmp(ptr);
                ptr--;
                return *this;
            }

            reference operator*() const { return ptr->key; }
            pointer operator->() const { return &ptr->key; }
            
            auto operator<=>(const iterator&) const = default;
            bool operator==(const iterator& rhs) const { return ptr == rhs.ptr; }
            bool operator!=(const iterator& rhs) const { return !(*this == rhs); }

            friend class SearchTree;
        };
        
        class const_iterator {
        private:
           CListIt ptr;
        
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = CListIt::difference_type;
            using value_type = const KeyT;
            using reference = value_type&;
            using pointer = value_type*;

        public:
            const_iterator(CListIt it = CListIt{}) : ptr(it) {}

            const_iterator& operator++() {
                ptr++;
                return *this;
            }

            const_iterator operator++(int) {
                const_iterator tmp(ptr);
                ptr++;
                return *this;
            }

            const_iterator& operator--() {
                ptr--;
                return *this;
            }

            const_iterator operator--(int) {
                const_iterator tmp(ptr);
                ptr--;
                return *this;
            }

            reference operator*() const { return ptr->key; }
            pointer operator->() const { return &ptr->key; }
            
            auto operator<=>(const const_iterator&) const = default;
            bool operator==(const const_iterator& rhs) const { return ptr == rhs.ptr; }
            bool operator!=(const const_iterator& rhs) const { return !(*this == rhs); }

            friend class SearchTree;
        };

        iterator begin() { return nodes.begin(); }
        iterator end() { return nil_; }
        const_iterator cbegin() const { return nodes.begin(); }
        const_iterator cend() const { return std::prev(nodes.cend()); }
        
    public:
        SearchTree(Comp comp = Comp()) : cmp(comp) {
            nodes.emplace_back(KeyT{}, 0, Color::Black, nodes.end(), nodes.end(), nodes.end());
            nil_ = std::prev(nodes.end());
            nil_->p = nil_;
            nil_->left = nil_;
            nil_->right = nil_;
            root_ = nil_;
        }
        SearchTree(std::initializer_list<KeyT> init, Comp comp = Comp()) : SearchTree(comp) {
            std::for_each(init.begin(), init.end(), [&](auto x) { insert(x); });
        }

    public:
        iterator find(KeyT key) const;
        iterator findMin() const;
        iterator findMax() const;
        iterator insert(KeyT key);
        iterator next(iterator it) const { return ++it; }
        void erase(const iterator it);

        int cnt(KeyT key) const;
        iterator nth_element(int n) const;

        std::vector<KeyT> getInorder() const { return std::vector<KeyT>(cbegin(), cend()); };
    private:
        ListIt next_impl(ListIt node) const;
        ListIt find_impl(ListIt node, KeyT key) const;
        ListIt findMin_impl(ListIt node) const;
        ListIt findMax_impl(ListIt node) const;

        void leftRotate(ListIt node);
        void rightRotate(ListIt node);

        ListIt insertFixup(ListIt node);
        void transplant(ListIt u, ListIt v);
        void eraseFixup(ListIt node);

        ListIt nth_element_impl(ListIt node, int n) const;

        bool compare(const KeyT& lhs, const KeyT& rhs) const { return cmp(lhs, rhs); }
        bool keys_eq(const KeyT& lhs, const KeyT& rhs) const { return !cmp(lhs, rhs) && !cmp(rhs, lhs); }
    };

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::find(KeyT key) const {
        return find_impl(root_, key);
    }

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::ListIt SearchTree<KeyT, Comp>::find_impl(ListIt node, KeyT key) const {
        while (node != nil_ && !keys_eq(key, node->key))
            if (compare(key, node->key))
                node = node->left;
            else
                node = node->right;

        return node;
    }

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::findMin() const {
        return findMin_impl(root_);
    }

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::ListIt SearchTree<KeyT, Comp>::findMin_impl(ListIt node) const {
        while (node->left != nil_)
            node = node->left;

        return node;
    }
    
    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::findMax() const {
        return findMax_impl(root_);
    }

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::ListIt SearchTree<KeyT, Comp>::findMax_impl(ListIt node) const {
        while (node->right != nil_)
            node = node->right;
            
        return node;
    }

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::ListIt SearchTree<KeyT, Comp>::next_impl(ListIt x) const {
        if (x->right != nil_)
            return findMin_impl(x->right);
        
        ListIt y = x->p;
        while (y != nil_ && x == y->right) {
            x = y;
            y = y->p;
        }

        return y;
    }

    template <typename KeyT, typename Comp>
    void SearchTree<KeyT, Comp>::leftRotate(ListIt x) {
        ListIt y = x->right;
        
        x->subtreeSize -= y->right->subtreeSize + 1;
        y->subtreeSize += x->left->subtreeSize + 1;

        x->right = y->left;
        if (y->left != nil_)
            y->left->p = x;
        y->p = x->p;
        if (x->p == nil_)
            root_ = y;
        else if (x == x->p->left)
            x->p->left = y;
        else
            x->p->right = y;
        y->left = x;
        x->p = y;
    }

    template <typename KeyT, typename Comp>
    void SearchTree<KeyT, Comp>::rightRotate(ListIt y) {
        ListIt x = y->left;

        x->subtreeSize += y->right->subtreeSize + 1;
        y->subtreeSize -= x->left->subtreeSize + 1;

        y->left = x->right;
        if (x->right != nil_)
            x->right->p = y;
        x->p = y->p;
        if (y->p == nil_)
            root_ = x;
        else if (y == y->p->left)
            y->p->left = x;
        else
            y->p->right = x;
        x->right = y;
        y->p = x;
    }

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::insert(KeyT key) {
        nodes.emplace_back(key, 1, Color::Red, nil_, nil_, nil_);
        ListIt z = std::prev(nodes.end());
        ListIt x = root_, y = nil_;

        while (x != nil_) {
            y = x;
            x->subtreeSize++;
            if (compare(z->key, x->key))
                x = x->left;
            else if (compare(x->key, z->key))
                x = x->right;
            else {
                nodes.pop_back();
                return nil_;
            }
        }

        z->p = y;
        if (y == nil_) {
            root_ = z;
            root_->subtreeSize = 1;
        }
        else if (compare(z->key, y->key))
            y->left = z;
        else
            y->right = z;
        
        insertFixup(z);
        nodes.splice(next_impl(z), nodes, z);
        return z;
    }

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::ListIt SearchTree<KeyT, Comp>::insertFixup(ListIt z) {
        while (z->p->color == Color::Red) {
            if (z->p == z->p->p->left) {
                ListIt y = z->p->p->right;
                if (y->color == Color::Red) {
                    z->p->color = Color::Black;
                    y->color = Color::Black;
                    z->p->p->color = Color::Red;
                    z = z->p->p;
                }
                else {
                    if (z == z->p->right) {
                        z = z->p;
                        leftRotate(z);
                    }
                    z->p->color = Color::Black;
                    z->p->p->color = Color::Red;
                    rightRotate(z->p->p);
                }
            }
            else {
                ListIt y = z->p->p->left;
                if (y->color == Color::Red) {
                    z->p->color = Color::Black;
                    y->color = Color::Black;
                    z->p->p->color = Color::Red;
                    z = z->p->p;
                }
                else {
                    if (z == z->p->left) {
                        z = z->p;
                        rightRotate(z);
                    }
                    z->p->color = Color::Black;
                    z->p->p->color = Color::Red;
                    leftRotate(z->p->p);
                }
            }
        }
        
        root_->color = Color::Black;
        return z;
    }

    template <typename KeyT, typename Comp>
    void SearchTree<KeyT, Comp>::transplant(ListIt u, ListIt v) {
        if (u->p == nil_) {
            root_ = v;
        }
        else if (u == u->p->left) {
            u->p->left = v;
        }
        else {
            u->p->right = v;
        }
        v->p = u->p;
    }

    template <typename KeyT, typename Comp>
	void SearchTree<KeyT, Comp>::erase(const iterator it) {
        if (it == end())
            throw std::runtime_error("end iterator passed to erase function in tree");
        ListIt z = it.ptr;
        ListIt node = root_;

        while (node != z) {
            node->subtreeSize--;
            if (compare(z->key, node->key))
                node = node->left;
            else
                node = node->right;
        }

        ListIt x, y = z;
        Color old_color = y->color;
        if (z->left == nil_) {
            x = z->right;
            transplant(z, z->right);
        }
        else if (z->right == nil_) {
            x = z->left;
            transplant(z, z->left);
        }
        else {
            y = findMin_impl(z->right);
            old_color = y->color;
            x = y->right;
            if (y != z->right) {
                transplant(y, y->right);
                y->right = z->right;
                y->right->p = y;
            }
            else
                x->p = y;
            
            transplant(z, y);
            y->left = z->left;
            y->left->p = y;
            y->color = z->color;
        }
        if (old_color == Color::Black) {
            eraseFixup(x);
        }
        nodes.erase(z);
    }

    template <typename KeyT, typename Comp>
    void SearchTree<KeyT, Comp>::eraseFixup(ListIt x) {
        ListIt w;
        while (x != root_ && x->color == Color::Black) {
            if (x == x->p->left) {
                w = x->p->right;
                if (w->color == Color::Red) {
                    w->color = Color::Black;
                    x->p->color = Color::Red;
                    leftRotate(x->p);
                    w = x->p->right;
                }
                if (w->left->color == Color::Black && w->right->color == Color::Black) {
                    w->color = Color::Red;
                    x = x->p;
                }
                else {
                    if (w->right->color == Color::Black) {
                        w->left->color = Color::Black;
                        w->color = Color::Red;
                        rightRotate(w);
                        w = x->p->right;
                    }
                    w->color = x->p->color;
                    x->p->color = Color::Black;
                    w->right->color = Color::Black;
                    leftRotate(x->p);
                    x = root_;
                }
            }
            else {
                w = x->p->left;
                if (w->color == Color::Red) {
                    w->color = Color::Black;
                    x->p->color = Color::Red;
                    rightRotate(x->p);
                    w = x->p->left;
                }
                if (w->right->color == Color::Black && w->left->color == Color::Black) {
                    w->color = Color::Red;
                    x = x->p;
                }
                else {
                    if (w->left->color == Color::Black) {
                        w->right->color = Color::Black;
                        w->color = Color::Red;
                        leftRotate(w);
                        w = x->p->left;
                    }
                    w->color = x->p->color;
                    x->p->color = Color::Black;
                    w->left->color = Color::Black;
                    rightRotate(x->p);
                    x = root_;
                }
            }
        }
        x->color = Color::Black;
    }

    template <typename KeyT, typename Comp>
    int SearchTree<KeyT, Comp>::cnt(KeyT key) const {
        ListIt node = root_, y = nil_;
        int res = root_->subtreeSize;
        while (node != nil_ && !keys_eq(key, node->key)) {
            y = node;
            if (compare(key, node->key)) {
                res -= node->right->subtreeSize + 1;
                node = node->left;
            } else {
                node = node->right;
            }
        }

        if (node != nil_ && keys_eq(key, node->key)) {
            res -= node->right->subtreeSize + 1;
        }

        return res;
    }

    template <typename KeyT, typename Comp>
    SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::nth_element(int n) const {
        if (root_->subtreeSize < n)
            return nil_;
        
        return nth_element_impl(root_, n);
    }

    template <typename KeyT, typename Comp>
    SearchTree<KeyT, Comp>::ListIt SearchTree<KeyT, Comp>::nth_element_impl(ListIt node, int n) const
    {
        while (node != nil_) {
            if (n == node->left->subtreeSize + 1)
                return node;
            if (n <= node->left->subtreeSize) {
                node = node->left;
            } else {
                n -= node->left->subtreeSize + 1;
                node = node->right;
            }
        }
        return nil_;
    }
}
