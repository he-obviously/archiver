#include "prefix_tree.h"

PrefixTree* Merge(PrefixTree* left, PrefixTree* right) {
    auto merged_tree = new PrefixTree();
    merged_tree->left_son_ = left;
    merged_tree->right_son_ = right;
    left->parent_ = merged_tree;
    right->parent_ = merged_tree;
    merged_tree->weight_ = left->weight_ + right->weight_;
    return merged_tree;
}