#pragma once

#include <cmath>
#include <vector>
#include "glm/gtc/matrix_transform.hpp"

class Geo {
public:
    virtual const void* getVertices() = 0;
    virtual const void* getIndices() = 0;
    virtual unsigned int getSize() = 0;
    virtual unsigned int getCount() = 0;
};


class Cube final : public Geo {
private:
    float* vertices;
    unsigned int* indices;

    unsigned int size = 8 * 8 * sizeof(float);
    unsigned int count = 12 * 3;
public:
    inline Cube() {
        vertices = new float[64] {
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, -0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  0.5f,  0.5f,  0.5f,
             0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f ,-0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f, -0.5f, -0.5f,
        };

        indices = new unsigned int[36] {
            0, 1, 2,
            0, 2, 3,
            5, 4, 7,
            5, 7, 6,
            4, 5, 1,
            4, 1, 0,
            6, 7, 3,
            6, 3, 2,
            1, 5, 6,
            1, 6, 2,
            4, 0, 3,
            4, 3, 7,
        };
    }

    inline ~Cube() {
        delete [] vertices;
        delete [] indices;
    }

    const void* getVertices() override {
        return vertices;
    }
    const void* getIndices() override {
        return indices;
    }
    unsigned int getSize() override {
        return size;
    };
    unsigned int getCount() override {
        return count;
    }
};

class Pyramid final : public Geo {
private:
    float* vertices;
    unsigned int* indices;

    unsigned int size = 5 * 8 * sizeof(float);
    unsigned int count = 6 * 3;
public:
    inline Pyramid() {
        vertices = new float[40] {
             0.0f,  0.5f,  0.0f, 0.5f, 1.0f,  0.0f,  0.5f,  0.0f,
             0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, -0.5f, -0.5f, -0.5f,
        };

        indices = new unsigned int[18] {
            0, 1, 2,
            0, 3, 1,
            0, 4, 3,
            0, 2, 4,
            3, 4, 2,
            3, 2, 1
        };
    }

    inline ~Pyramid() {
        delete [] vertices;
        delete [] indices;
    }

    const void* getVertices() override {
        return vertices;
    }
    const void* getIndices() override {
        return indices;
    }
    unsigned int getSize() override {
        return size;
    };
    unsigned int getCount() override {
        return count;
    }
};

class Sphere final : public Geo {
private:
    int numOfVertices;
    int numOfIndices;
    float* vertices;
    std::vector<int> indices;
public:
    inline Sphere(int precision = 48) {
        init(precision);
    }

    inline ~Sphere() {
        indices.clear();
        delete [] vertices;
    }
private:
    inline void init(int precision) {
        numOfVertices = (precision + 1) * (precision + 1);
        numOfIndices = precision * precision * 6;
        indices.reserve(numOfIndices);
        vertices = new float[numOfVertices * (3 + 2 + 3)];

        for (int i = 0; i <= precision; i++) {
            for (int j = 0; j <= precision; j++) {
                float y = (float)cos(glm::radians(180.0f - i * 180.0f / precision));
                float x = -(float)cos(glm::radians(j * 360.0f / precision)) * (float)abs(cos(asin(y)));
                float z = (float)sin(glm::radians(j * 360.0f / precision)) * (float)abs(cos(asin(y)));

                int index = i * (precision + 1) + j;
                vertices[index * 8 + 0] = x; 
                vertices[index * 8 + 1] = y; 
                vertices[index * 8 + 2] = z; 
                vertices[index * 8 + 3] = (float)j / precision; 
                vertices[index * 8 + 4] = (float)i / precision; 
                vertices[index * 8 + 5] = x; 
                vertices[index * 8 + 6] = y; 
                vertices[index * 8 + 7] = z; 
            }
        }

        for (int i = 0; i < precision; i++) {
            for (int j = 0; j < precision; j++) {
                indices[6 * (i * precision + j) + 0] = i * (precision + 1) + j;
                indices[6 * (i * precision + j) + 1] = i * (precision + 1) + j + 1;
                indices[6 * (i * precision + j) + 2] = (i + 1) * (precision + 1) + j;
                indices[6 * (i * precision + j) + 3] = i * (precision + 1) + j + 1;
                indices[6 * (i * precision + j) + 4] = (i + 1) * (precision + 1) + j + 1;
                indices[6 * (i * precision + j) + 5] = (i + 1) * (precision + 1) + j;
            }
        }
    }
public:
    const void* getVertices() override { return vertices; }

    inline const void* getIndices() override { return indices.data(); }

    inline unsigned int getSize() override { return numOfVertices * sizeof(float) * 8; }

    inline unsigned int getCount() override { return numOfIndices; }
};

class Torus final {
private:
    float _precision;
    float _innner;
    float _outer;

    int numOfVertices;
    int numOfIndices;
    float* vertices;
    std::vector<int> indices;
public:
    inline Torus(float precision, float inner, float outer) {
        init(precision, inner, outer);
    }
    inline ~Torus() {
        delete [] vertices;
        indices.clear();
    }
private:
    inline void init(float precision, float inner, float outer) {
        numOfVertices = (precision + 1) * (precision + 1);
        numOfIndices = precision * precision * 6;
        indices.reserve(numOfIndices);
        vertices = new float[numOfVertices * (3 + 2 + 3)];
        for (int i = 0; i <= precision; i++) {
            glm::mat4 rMat= glm::rotate(glm::mat4(1.0f), glm::radians(i * 360.0f / precision), glm::vec3(0.0, 0.0, 1.0f));

        }
    }
};