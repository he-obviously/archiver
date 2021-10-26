#include "read.h"

std::vector<std::vector<size_t>> ConvertFilesIntoVector(int argc, char* file_names[]) {
    std::vector<std::vector<size_t>> counter(argc, std::vector<size_t>(ALPHABET_SIZE, 0));
    for (size_t i = 4; i < argc; ++i) {
        counter[i][FILENAME_END] = 1;
        counter[i][ONE_MORE_FILE] = 1;
        counter[i][ARCHIVE_END] = 1;
        char* file_name = file_names[i];
        std::ifstream fin(file_name);
        std::string_view file_name_copy = file_name;
        for (uint8_t symbol: file_name_copy) {
            ++counter[i][symbol];
        }
        while (true) {
            uint8_t current_byte = fin.get();
            if (fin.eof()) {
                break;
            }
            ++counter[i][current_byte];
        }
    }
    return counter;
}
