#pragma once

#include <fstream>
#include <cstdio>
#include <sstream>
#include <string>

inline void loadShaderSource(const char* path, char* dst, unsigned int size) {
    memset(dst, 1, sizeof(char) *  size);
    std::ifstream stream(path, std::ios::in);
    std::string line = "";
    std::stringstream ss;
    while (getline(stream, line)) {
        ss << line << "\n";
    }
    if (!stream.is_open()) {
        printf("\x1b[31;1m[Open File Error] Failed to open file: %s\n\x1b[0m", path);
    }

    std::string content = ss.str();
    size_t length = content.copy(dst, content.length());
    dst[length] = '\0';

    stream.close();
}

inline void writeShaderSource(const char* path, char* src) {
    std::ofstream stream(path, std::ios::trunc);
    if (stream.is_open()) {
        stream << src;

        printf("\x1b[32;1m[Write File Info]  Write to file: %s\n\x1b[0m", path);
    } else {
        printf("\x1b[31;1m[Write File Error] Failed to write file: %s\n\x1b[0m", path);
    }

    stream.close();
}