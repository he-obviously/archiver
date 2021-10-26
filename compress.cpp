#include "compress.h"
#include <queue>
#include "prefix_tree.h"
#include <iostream>
#include <algorithm>
#include "bin_write.h"

using element = std::pair<size_t, PrefixTree*>;
using element_priority_queue = std::priority_queue<element, std::vector<element>, std::greater<>>;

void BuildPrefixTree(element_priority_queue& queue) {
    while (queue.size() > 1) {
        PrefixTree* first_to_merge = queue.top().second;
        queue.pop();
        PrefixTree* second_to_merge = queue.top().second;
        queue.pop();
        PrefixTree* merged_tree = Merge(first_to_merge, second_to_merge);
        queue.push(std::make_pair(merged_tree->weight_, merged_tree));
    }
}

std::vector<std::pair<uint16_t, uint16_t>> GetCodeLengths(const std::vector<PrefixTree*>& nodes) {
    std::vector<std::pair<uint16_t, uint16_t>> map;
    for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
        if (nodes[i] != nullptr) {
            uint16_t length = 0;
            PrefixTree* current_node = nodes[i];
            while (current_node->parent_ != nullptr) {
                ++length;
                current_node = current_node->parent_;
            }
            map.emplace_back(static_cast<uint16_t>(i), length);
        }
    }
    return map;
}

bool Comp(const std::pair<uint16_t, uint16_t>& a, const std::pair<uint16_t, uint16_t>& b) {
    if (a.second == b.second) {
        return a.first < b.first;
    } else {
        return a.second < b.second;
    }
}

void compress(int argc, char* file_names[]) {
    std::ofstream fout;
    fout.open(file_names[3]);
    fout.close();
    uint8_t bits_occupied = 0;
    uint8_t current_char = 0;
    std::vector<std::vector<size_t>> frequency = ConvertFilesIntoVector(argc, file_names);
    for (int file_i = 4; file_i < argc; ++file_i) {
        std::vector<PrefixTree *> nodes_for_bytes(ALPHABET_SIZE, nullptr);
        element_priority_queue queue;
        for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
            if (frequency[file_i][i] != 0) {
                nodes_for_bytes[i] = new PrefixTree(frequency[file_i][i]);
                queue.push(std::make_pair(frequency[file_i][i], nodes_for_bytes[i]));
            }
        }
        BuildPrefixTree(queue);
        std::vector<std::pair<uint16_t, uint16_t>> sorted_code_lengths = GetCodeLengths(nodes_for_bytes);
        std::sort(sorted_code_lengths.begin(), sorted_code_lengths.end(), Comp);
        for (const auto& p : sorted_code_lengths) {
        }
        write(sorted_code_lengths, file_names[file_i], file_names[3],
              bits_occupied, current_char, argc - file_i - 1);
    }
}
