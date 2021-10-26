#include <cstdint>
#include <fstream>
#include "bin_read.h"

void decompress(char* archive_name) {
    std::ifstream fin(archive_name);
    uint16_t bits_occupied = 0;
    uint16_t current_code = 0;
    while(read(fin, bits_occupied, current_code)) {}
}
