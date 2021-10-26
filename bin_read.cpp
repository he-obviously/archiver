#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include "constants.h"
#include <algorithm>
#include "bin_read.h"
#include "bin_write.h"

namespace bin {
    uint8_t ReverseByte(uint8_t byte) {
        uint8_t reversed_byte = 0;
        for (uint i = 0; i < 8; ++i) {
            reversed_byte |= ((byte & 1) << (7 - i));
            byte >>= 1;
        }
        return reversed_byte;
    }

    bool ReadBit(std::ifstream& fin, uint16_t& bits_occupied, uint16_t& current_code) {
        if (bits_occupied != 0) {
            bool bit = (current_code & 1);
            current_code >>= 1;
            --bits_occupied;
            return bit;
        } else {
            uint8_t other_byte = fin.get();
            other_byte = ReverseByte(other_byte);
            current_code += (static_cast<uint16_t>(other_byte) << bits_occupied);
            bits_occupied += 8;
            return ReadBit(fin, bits_occupied, current_code);
        }
    }

    uint16_t ReadCode(std::ifstream& fin, uint16_t length, uint16_t& bits_occupied,
                      uint16_t& current_code) {
        uint16_t code = 0;
        for (uint16_t i = 0; i < length; ++i) {
            bool bit = ReadBit(fin, bits_occupied, current_code);
            code |= (bit << i);
        }
        return code;
    }

    uint16_t GetSymbol(std::ifstream& fin, uint16_t& bits_occupied, uint16_t& current_code,
              BinaryTree* head) {
        BinaryTree* current_node = head;
        while (true) {
            if (current_node->left_son == nullptr && current_node->right_son == nullptr) {
                return current_node->symbol;
            }
            bool bit = ReadBit(fin, bits_occupied, current_code);
            if (!bit) {
                current_node = current_node->left_son;
            } else {
                current_node = current_node->right_son;
            }
        }
    }
}

std::vector<std::pair<uint16_t, uint16_t>> GetCodeLengths(uint16_t symbols_count, std::ifstream& fin,
                                                          uint16_t& bits_occupied, uint16_t& current_code) {
    std::vector<std::pair<uint16_t, uint16_t>> code_lengths;
    for(uint16_t i = 0; i < symbols_count; ++i) {
        uint16_t symbol = bin::ReadCode(fin, 9, bits_occupied, current_code);
        code_lengths.emplace_back(symbol, 0);
    }
    size_t set_lengths = 0;
    uint16_t current_length = 0;
    while (set_lengths < code_lengths.size()) {
        ++current_length;
        uint16_t count_of_symbols = bin::ReadCode(fin, 9, bits_occupied, current_code);
        for (size_t i = set_lengths; i < set_lengths + static_cast<size_t>(count_of_symbols); ++i) {
            code_lengths[i].second = current_length;
        }
        set_lengths += count_of_symbols;
    }
    return code_lengths;
}

BinaryTree* BuildBinaryTree(const std::vector<Code>& codes) {
    auto* head = new BinaryTree();
    for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
        BinaryTree* current_node = head;
        if (codes[i].length == 0) {
            continue;
        }
        uint16_t code_copy = codes[i].code;
        for (uint16_t j = 0; j < codes[i].length; ++j) {
            bool bit = (code_copy & 1);
            if (!bit) {
                if (current_node->left_son == nullptr) {
                    current_node->left_son = new BinaryTree();
                }
                current_node = current_node->left_son;
            } else {
                if (current_node->right_son == nullptr) {
                    current_node->right_son = new BinaryTree();
                }
                current_node = current_node->right_son;
            }
            code_copy >>= 1;
        }
        current_node->symbol = static_cast<uint16_t>(i);
    }
    return head;
}

std::string GetFileName(std::ifstream& fin, uint16_t& bits_occupied, uint16_t& current_code,
                        BinaryTree* head) {
    std::string file_name;
    while (true) {
        uint16_t current_symbol = bin::GetSymbol(fin, bits_occupied, current_code, head);
        if (current_symbol == FILENAME_END) {
            return file_name;
        } else {
            file_name += static_cast<uint8_t>(current_symbol);
        }
    }
}

bool read(std::ifstream& fin, uint16_t& bits_occupied, uint16_t& current_code) {
    uint16_t symbols_count = bin::ReadCode(fin, 9, bits_occupied, current_code);
    std::vector<std::pair<uint16_t, uint16_t>> code_lengths =
            GetCodeLengths(symbols_count, fin, bits_occupied, current_code);
    std::vector<Code> codes = GetCodes(code_lengths);
    BinaryTree* head = BuildBinaryTree(codes);
    while (true) {
        std::ofstream fout(GetFileName(fin, bits_occupied, current_code, head));
        while (true) {
            uint16_t current_symbol = bin::GetSymbol(fin, bits_occupied, current_code, head);
            if (current_symbol == ARCHIVE_END) {
                fout.close();
                return false;
            } else if (current_symbol == ONE_MORE_FILE) {
                fout.close();
                return true;
            } else {
                fout.put(static_cast<uint8_t>(current_symbol));
            }
        }
    }
}