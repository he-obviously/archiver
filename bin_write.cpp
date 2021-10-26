#include <cstdint>
#include "bin_write.h"
#include <fstream>
#include "constants.h"
#include <iostream>

std::vector<Code> GetCodes(const std::vector<std::pair<uint16_t, uint16_t>>& code_lengths) {
    std::vector<Code> codes(ALPHABET_SIZE, {0, 0});
    uint16_t code = 0;
    for (size_t i = 0; i < code_lengths.size(); ++i) {
        uint16_t reversed_code = 0;
        for (uint16_t j = 0; j < code_lengths[i].second; ++j) {
            if (code & (1 << j)) {
                reversed_code += (1 << (code_lengths[i].second - j - 1));
            }
        }
        codes[static_cast<size_t>(code_lengths[i].first)] = {code_lengths[i].second, reversed_code};
        if (i + 1 < code_lengths.size()) {
            code = (code + 1) << (code_lengths[i + 1].second - code_lengths[i].second);
        }
    }
    return codes;
}

std::vector<uint16_t> GetCountLengths(const std::vector<std::pair<uint16_t, uint16_t>>& code_lengths) {
    std::vector<uint16_t> count_lengths;
    for (const auto& code_length : code_lengths) {
        while (count_lengths.size() < static_cast<size_t>(code_length.second)) {
            count_lengths.push_back(0);
        }
        ++count_lengths[code_length.second - 1];
    }
    return count_lengths;
}

namespace bin {
    void WriteBit(std::ofstream &fout, bool bit, uint8_t &bits_occupied, uint8_t &current_char) {
        if (bits_occupied == 8) {
            fout.put(current_char);
            current_char = 0;
            bits_occupied = 0;
        }
        current_char |= (bit << (7 - bits_occupied));
        ++bits_occupied;
    }

    void WriteCode(std::ofstream &fout, uint16_t code, uint16_t length,
                   uint8_t &bits_occupied, uint8_t &current_char) {
        for (uint16_t i = 0; i < length; ++i) {
            bool bit = code & 1;
            WriteBit(fout, bit, bits_occupied, current_char);
            code >>= 1;
        }
    }

    void WriteFile(std::ofstream &fout, char* file_name, const std::vector<Code>& codes,
                   uint8_t& bits_occupied, uint8_t& current_char) {
        std::ifstream fin(file_name);
        std::string_view file_name_copy = file_name;
        for (char i : file_name_copy) {
            auto symbol_pos = static_cast<uint8_t>(i);
            WriteCode(fout, codes[symbol_pos].code, codes[symbol_pos].length,
                      bits_occupied, current_char);
        }
        WriteCode(fout, codes[FILENAME_END].code, codes[FILENAME_END].length,
                  bits_occupied, current_char);
        while (true) {
            uint8_t current_byte = fin.get();
            if (fin.eof()) {
                break;
            }
            auto byte_pos = static_cast<size_t>(current_byte);
            WriteCode(fout, codes[byte_pos].code, codes[byte_pos].length,
                      bits_occupied, current_char);
        }
    }
}

void write(const std::vector<std::pair<uint16_t, uint16_t>>& code_lengths, char* file_name,
           char* archive_name, uint8_t& bits_occupied, uint8_t& current_char, int files_left) {
    std::ofstream fout(archive_name, std::ios::app);
    bin::WriteCode(fout, static_cast<uint16_t>(code_lengths.size()), 9, bits_occupied, current_char);
    for (const auto& [symbol, length] : code_lengths) {
        bin::WriteCode(fout, symbol, 9, bits_occupied, current_char);
    }
    std::vector<uint16_t> count_length = GetCountLengths(code_lengths);
    for (uint16_t count : count_length) {
        bin::WriteCode(fout, count, 9, bits_occupied, current_char);
    }
    std::vector<Code> codes = GetCodes(code_lengths);
    bin::WriteFile(fout, file_name, codes, bits_occupied, current_char);
    if (files_left == 0) {
        bin::WriteCode(fout, codes[ARCHIVE_END].code, codes[ARCHIVE_END].length,
                       bits_occupied, current_char);
        fout.put(current_char);
    } else {
        bin::WriteCode(fout, codes[ONE_MORE_FILE].code, codes[ONE_MORE_FILE].length,
                       bits_occupied, current_char);
    }
}
