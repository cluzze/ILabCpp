#pragma once

#include <functional>
#include <iostream>
#include <list>
#include <stack>

namespace containers
{
    template <typename KeyT, typename Comp = std::less<KeyT>>
	class SearchTree {
    private:
        enum class Color {
            Black = 0,
            Red = 1
        };

        struct TreeNode;
        using ListIt = std::list<TreeNode>::iterator;
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
        using iterator = ListIt;
        using const_iterator = std::list<TreeNode>::const_iterator;

        iterator begin() { return nodes.begin(); }
        iterator end() { return nil_; }
        const_iterator cbegin() const { return nodes.cbegin(); }
        const_iterator cend() const { return nil_; }

    public:
        SearchTree();

    public:
        iterator find(KeyT key) const;
        iterator findMin() const;
        iterator findMax() const;
        iterator next(iterator node) const;
        iterator insert(KeyT key);
        void erase(const iterator node);

        int cnt(KeyT key) const;
        iterator nth_element(int n) const;

        void printInOrder() const;
        void dump() const;
    private:
        iterator find_impl(iterator node, KeyT key) const;
        iterator findMin_impl(iterator node) const;
        iterator findMax_impl(iterator node) const;

        void leftRotate(iterator node);
        void rightRotate(iterator node);

        iterator insertFixup(iterator node);
        void transplant(iterator u, iterator v);
        void eraseFixup(iterator node);
        void dump_impl(iterator node) const;

        iterator nth_element_impl(ListIt node, int n) const;

    };

    template <typename KeyT, typename Comp>
    SearchTree<KeyT, Comp>::SearchTree() {
        nodes.emplace_back(KeyT{}, 0, Color::Black, nodes.end(), nodes.end(), nodes.end());
        nil_ = std::prev(nodes.end());
        nil_->p = nil_;
        nil_->left = nil_;
        nil_->right = nil_;
        root_ = nil_;
    }

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::find(KeyT key) const {
        return find_impl(root_, key);
    }

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::find_impl(iterator node, KeyT key) const {
        while (node != nil_ && Comp()(key, node->key) != Comp()(node->key, key))
            if (Comp()(key, node->key))
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
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::findMin_impl(iterator node) const {
        while (node->left != nil_)
            node = node->left;

        return node;
    }
    
    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::findMax() const {
        return findMax_impl(root_);
    }

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::findMax_impl(iterator node) const {
        while (node->right != nil_)
            node = node->right;
            
        return node;
    }

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::next(iterator x) const {
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
    void SearchTree<KeyT, Comp>::leftRotate(iterator x) {
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
    void SearchTree<KeyT, Comp>::rightRotate(iterator y) {
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
            if (Comp()(z->key, x->key))
                x = x->left;
            else if (Comp()(x->key, z->key))
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
        else if (Comp()(z->key, y->key))
            y->left = z;
        else
            y->right = z;
        
        insertFixup(z);
        nodes.splice(next(z), nodes, z);
        return z;
    }

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::insertFixup(iterator z) {
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
    void SearchTree<KeyT, Comp>::transplant(iterator u, iterator v) {
        if (u->p == nil_) {
            root_ = v;
            //root_->subtreeSize = v->subtreeSize;
        }
        else if (u == u->p->left) {
            u->p->left = v;
            //u->p->subtreeSize += v->subtreeSize - u->subtreeSize;
        }
        else {
            u->p->right = v;
            //u->p->subtreeSize += v->subtreeSize - u->subtreeSize;
        }
        v->p = u->p;
    }

    template <typename KeyT, typename Comp>
	void SearchTree<KeyT, Comp>::erase(const iterator z) {
        if (z == cend())
            throw std::runtime_error("end iterator passed to erase function in tree");
        ListIt node = root_;
        
        while (node != z) {
            node->subtreeSize--;
            if (Comp()(z->key, node->key))
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
    void SearchTree<KeyT, Comp>::eraseFixup(iterator x) {
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
    void SearchTree<KeyT, Comp>::printInOrder() const {
        auto x = nodes.begin();
        for (; x != nodes.end(); ++x) {
            std::cout << x->key << ' ';
        }
        std::cout << '\n';
    }

    template <typename KeyT, typename Comp>
    void SearchTree<KeyT, Comp>::dump() const {
        if (root_ == nil_) {
            std::cout << "tree is empty\n";
            return;
        }

        dump_impl(root_);
    }

    template <typename KeyT, typename Comp>
    void SearchTree<KeyT, Comp>::dump_impl(iterator node) const {
        char c1 = node->color == Color::Black ? 'b' : 'r';
        std::cout << "node: " << node->key << c1 << node->subtreeSize << ' ';
        if (node->p == nil_)
            std::cout << "p: " << "nil" << ' ';
        else {
            char c = node->p->color == Color::Black ? 'b' : 'r';
            std::cout << "p: " << node->p->key << c << ' ';
        }
        if (node->left == nil_)
            std::cout << "left: " << "nil" << ' ';
        else {
            char c = node->left->color == Color::Black ? 'b' : 'r';
            std::cout << "left: " << node->left->key << c << ' ';
        }
        if (node->right == nil_)
            std::cout << "right: " << "nil" << ' ';
        else {
            char c = node->right->color == Color::Black ? 'b' : 'r';
            std::cout << "right: " << node->right->key << c << ' ';
        }
        std::cout << '\n';
        if (node->left != nil_)
            dump_impl(node->left);
        if (node->right != nil_)
            dump_impl(node->right);
    }

    template <typename KeyT, typename Comp>
    int SearchTree<KeyT, Comp>::cnt(KeyT key) const {
        ListIt node = root_, y = nil_;
        int res = root_->subtreeSize;
        while (node != nil_ && key != node->key) {
            y = node;
            if (Comp()(key, node->key)) {
                res -= node->right->subtreeSize + 1;
                node = node->left;
            } else {
                node = node->right;
            }
        }

        if (node != nil_ && key == node->key) {
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
    SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::nth_element_impl(ListIt node, int n) const
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
