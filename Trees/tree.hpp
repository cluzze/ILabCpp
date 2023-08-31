#pragma once

#include <functional>

namespace containers
{
    template <typename KeyT, typename Comp = std::less<KeyT>>
	class SearchTree {
    private:
        enum class Color {
            Black = 0,
            Red = 1
        };

        struct TreeNode {
            KeyT key;
            Color color;
            TreeNode *p, *left, *right;
        };
        
        using iterator = TreeNode*;
        TreeNode *root_;
    public:
        SearchTree();

    public:
        iterator find(KeyT key);
        iterator findMin();
        iterator findMax();
        iterator next(iterator node);
        iterator insert(KeyT key);
        void erase(iterator node);

    private:
        iterator find_impl(iterator node, KeyT key);
        iterator findMin_impl(iterator node);
        iterator findMax_impl(iterator node);

        void leftRotate(iterator node);
        void rightRotate(iterator node);

        iterator insertFixup(iterator node);
        
    };

    template <typename KeyT, typename Comp>
    SearchTree<KeyT, Comp>::SearchTree() {

    }

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::find(KeyT key) {
        return find_impl(root_, key);
    }

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::find_impl(iterator node, KeyT key) {
        while (node && key != node->key)
            if (Comp(key, node->key))
                node = node->left;
            else
                node = node->right;

        return node;
    }

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::findMin() {
        return findMix_impl(root_);
    }

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::findMin_impl(iterator node) {
        while (node)
            node = node->left;

        return node;
    }
    
    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::findMax() {
        return findMax_impl(root_);
    }

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::findMax_impl(iterator node) {
        while (node)
            node = node->right;
            
        return node;
    }

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::next(iterator node) {
        if (node->right)
            return findMin_impl(node->right);
        
        TreeNode *y = node->p;
        while (y && node == y->right) {
            node = y;
            y = node->p;
        }

        return y;
    }

    template <typename KeyT, typename Comp>
    void SearchTree<KeyT, Comp>::leftRotate(iterator x) {
        TreeNode *y = x->right;
        x->right = y->left;
        if (y->left)
            y->left->p = x;
        y->p = x->p;
        if (x->p == nullptr)
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
        TreeNode *x = y->left;
        y->left = x->right;
        if (x->right)
            x->right->p = y;
        x->p = y->p;
        if (y->p == nullptr)
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
        TreeNode *z = new TreeNode{key, Color::Red, nullptr, nullptr, nullptr};
        TreeNode *x = root_, *y = nullptr;

        while (x) {
            y = x;
            if (Comp(z->key, x->key))
                x = x->left;
            else
                x = x->right;
        }

        z->p = y;
        if (!y)
            root_ = z;
        else if (Comp(z->key, y->key))
            y->left = z;
        else
            y->right = z;
        
        z = insertFixup(z);

        return z;
    }

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::insertFixup(iterator z) {
        while (z->p->color == Color::Red) {
            if (z->p == z->p->p->left) {
                TreeNode *y = z->p->p->right;
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
                TreeNode *y = z->p->p->left;
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
	void SearchTree<KeyT, Comp>::erase(iterator node) {

    }

}
