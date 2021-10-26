#pragma once

#include <cstdio>
#include <vector>

class PrefixTree {
public:
    PrefixTree* left_son_ = nullptr;
    PrefixTree* right_son_ = nullptr;
    PrefixTree* parent_ = nullptr;
    size_t weight_ = 0;
    PrefixTree() = default;
    explicit PrefixTree(size_t weight) {
        weight_ = weight;
    }
    PrefixTree(PrefixTree* left, PrefixTree* right, size_t weight) {
        left_son_ = left;
        right_son_ = right;
        weight_ = weight;
    }
};

PrefixTree* Merge(PrefixTree* left, PrefixTree* right);