#pragma once

struct BinaryTree {
    BinaryTree* left_son = nullptr;
    BinaryTree* right_son = nullptr;
    uint16_t symbol = '\0';
    BinaryTree() = default;
    explicit  BinaryTree(uint16_t symbol_) {
        symbol = symbol_;
    }
    BinaryTree(BinaryTree* left, BinaryTree* right, uint16_t symbol_) {
        left_son = left;
        right_son = right;
        symbol = symbol_;
    }
};

bool read(std::ifstream& fin, uint16_t& bits_occupied, uint16_t& current_byte);
