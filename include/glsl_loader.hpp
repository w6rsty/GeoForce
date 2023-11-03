#pragma once

#include <fstream>
#include <cstdio>
#include <sstream>
#include <string>
#include "imgui.h"

const unsigned int EDITOR_BUFFER_SIZE = 2048;


inline void loadShaderSource(const char* path, char* dst) {
    memset(dst, 0, sizeof(char) *  EDITOR_BUFFER_SIZE);
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