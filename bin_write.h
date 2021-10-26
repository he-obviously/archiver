#pragma once

#include <vector>

struct Code {
    uint16_t length = 0;
    uint16_t code = 0;
    Code() = default;
    Code(uint16_t length_, uint16_t code_) {
        length = length_;
        code = code_;
    }
};

void write(const std::vector<std::pair<uint16_t, uint16_t>>& code_lengths, char* file_name,
           char* archive_name, uint8_t& bits_occupied, uint8_t& current_char, int files_left);

std::vector<Code> GetCodes(const std::vector<std::pair<uint16_t, uint16_t>>& code_lengths);