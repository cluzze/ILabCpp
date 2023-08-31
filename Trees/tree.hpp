#pragma once

#include <functional>

namespace containers
{
    template <typename KeyT, typename Comp = std::less<KeyT>>
	class SearchTree {
    private:
        struct TreeNode {
            KeyT key;
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
	SearchTree<KeyT, Comp>::iterator SearchTree<KeyT, Comp>::insert(KeyT key) {
        TreeNode *new_node = new TreeNode{key, nullptr, nullptr, nullptr};
        TreeNode *x = root_, *y = nullptr;

        while (x) {
            y = x;
            if (Comp(key, x->key))
                x = x->left;
            else
                x = x->right;
        }

        new_node->p = y;
        if (!y)
            root_ = new_node;
        else if (Comp(key, y->key))
            y->left = new_node;
        else
            y->right = new_node;
        
        return new_node;
    }

    template <typename KeyT, typename Comp>
	void SearchTree<KeyT, Comp>::erase(iterator node) {
        
    }

}
