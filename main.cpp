#include <iostream>
#include <cstring>
#include "compress.h"
#include "decompress.h"

int main(int argc, char* argv[]) {
    try {
        if (argc < 3) {
            throw "Error! Too few arguments.";
        }
        if (std::strcmp(argv[1], FIRST_COMMAND) != 0 || (std::strcmp(argv[2], SHOW_HELP) != 0 &&
                std::strcmp(argv[2], COMPRESS) != 0 && std::strcmp(argv[2], DECOMPRESS) != 0)) {
            throw "Error! Invalid command.";
        }
    }
    catch (const char* error) {
        std::cout << error << std::endl;
        return 1;
    }
    if (std::strcmp(argv[2], SHOW_HELP) == 0) {
        try {
            if (argc > 3) {
                throw "Error! Invalid command.";
            }
        }
        catch (const char* error) {
            std::cout << error << std::endl;
            return 1;
        }
        std::cout << HELP_TEXT;
    } else if (std::strcmp(argv[2], COMPRESS) == 0) {
        compress(argc, argv);
    } else {
        try {
            if (argc > 4) {
                throw "Error! Invalid command.";
            }
        }
        catch (const char* error) {
            std::cout << error << std::endl;
            return 1;
        }
        decompress(argv[3]);
    }
    return 0;
}