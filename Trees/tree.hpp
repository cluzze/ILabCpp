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
        TreeNode *nil_;
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
        void transplant(iterator u, iterator v);
        iterator eraseFixup(iterator node);

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
        while (node != nil_ && key != node->key)
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
        while (node != nil_)
            node = node->left;

        return node;
    }
    
    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::findMax() {
        return findMax_impl(root_);
    }

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::findMax_impl(iterator node) {
        while (node != nil_)
            node = node->right;
            
        return node;
    }

    template <typename KeyT, typename Comp>
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::next(iterator x) {
        if (x->right != nil_)
            return findMin_impl(x->right);
        
        TreeNode *y = node->p;
        while (y != nil_ && x == y->right) {
            x = y;
            y = y->p;
        }

        return y;
    }

    template <typename KeyT, typename Comp>
    void SearchTree<KeyT, Comp>::leftRotate(iterator x) {
        TreeNode *y = x->right;
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
        TreeNode *x = y->left;
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
        TreeNode *z = new TreeNode{key, Color::Red, nil_, nil_, nil_};
        TreeNode *x = root_, *y = nil_;

        while (x != nil_) {
            y = x;
            if (Comp(z->key, x->key))
                x = x->left;
            else
                x = x->right;
        }

        z->p = y;
        if (y == nil_)
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
    void SearchTree<KeyT, Comp>::transplant(iterator u, iterator v) {
        if (u->p == nil_)
            root_ = v;
        else if (u == u->p->left)
            u->p->left = v;
        else
            u->p->right = v;
        v->p = u->p;
    }

    template <typename KeyT, typename Comp>
	void SearchTree<KeyT, Comp>::erase(iterator z) {
        TreeNode *x, *y = z;
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
    }

    template <typename KeyT, typename Comp>
    SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::eraseFixup(iterator x) {
        TreeNode *w = nullptr;
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
                if (w->right->color = Color::Black && w->left->color == Color::Black) {
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
}
