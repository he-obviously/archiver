#pragma once

#include <cstddef>
#include <cstdint>

static const char* FIRST_COMMAND = "archiver";
static const char* SHOW_HELP = "-h";
static const char* COMPRESS = "-c";
static const char* DECOMPRESS = "-d";
static const char* HELP_TEXT = "archiver -c archive_name file1 [file2 ...] - заархивировать файлы fil1, file2, ... "
                  "и сохранить результат в файл archive_name.\n"
                  "archiver -d archive_name - разархивировать файлы из архива archive_name и положить в текущую директорию.\n"
                  "archiver -h - вывести справку по использованию программы.";
static const size_t ALPHABET_SIZE = static_cast<size_t>(259);
static const uint16_t FILENAME_END = static_cast<uint16_t>(256);
static const uint16_t ONE_MORE_FILE = static_cast<uint16_t>(257);
static const uint16_t ARCHIVE_END = static_cast<uint16_t>(258);