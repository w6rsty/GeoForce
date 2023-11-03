#pragma once

#include "glad/glad.h"
#include "stb_image.h"
#include <string>
#include <random>


class Texture final {
private:
    unsigned int texture_id;
    std::string file_path;
    unsigned char* data;
    int width, height, BPP;
public:
    Texture(const std::string& filePath);
    Texture(int width, int height, const unsigned char* data);
    ~Texture();
    void Bind(unsigned int slot = 0) const;
    void Unbind() const;
};

inline void createCheckboardTexture(unsigned char* textureBuffer, int width, int height, int tileSize) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int cellColor = ((i / tileSize) + (j / tileSize)) % 2 == 0 ? 255 : 0;
            int index = (i * width + j) * 3; // 每个像素3个通道（R,G,B）

            textureBuffer[index] = cellColor;
            textureBuffer[index + 1] = cellColor;
            textureBuffer[index + 2] = cellColor;
        }
    }
}

// 生成柏林噪声
inline int berlinNoise(int x, int y) {
    int n = x * 671 + y * 1511;
    n = (n << 13) ^ n;
    return (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
}

inline void createBerlinNoiseTexture(unsigned char* textureBuffer, int width, int height, int tileSize) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 255);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int noiseValue = berlinNoise(j / tileSize, i / tileSize) % 256;
            int index = (i * width + j) * 3; // 每个像素3个通道（R,G,B）

            int grayValue = dist(rng) * noiseValue / 255;

            textureBuffer[index] = grayValue;
            textureBuffer[index + 1] = grayValue;
            textureBuffer[index + 2] = grayValue;
        }
    }
}