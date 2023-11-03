#pragma once

#include "glad/glad.h"

#include "vertex_buffer.hpp"
#include <vector>

struct VertexBufferElement;
class VertexBufferLayout;

class VertexArray final {
private:
    unsigned int buffer_id;
public:
    VertexArray();
    ~VertexArray();
    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void Bind() const;
    void Unbind() const;
};

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getSize(unsigned int type) {
        switch (type) {
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        return 0;
    }
};

class VertexBufferLayout {
private:
    std::vector<VertexBufferElement> elements;
    unsigned int stride;
public:
    VertexBufferLayout() : stride(0) {}

    void push_float(unsigned int count) {
        elements.push_back({ GL_FLOAT, count, GL_FALSE});
        stride += VertexBufferElement::getSize(GL_FLOAT) * count;
    }

    void push_unsigned_int(unsigned int count) {
        elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
        stride += VertexBufferElement::getSize(GL_UNSIGNED_INT) * count;
    }

    void push_unsigned_byte(unsigned int count) {
        elements.push_back({ GL_UNSIGNED_BYTE, count, GL_FALSE });
        stride += VertexBufferElement::getSize(GL_UNSIGNED_BYTE) * count;
    }

    inline std::vector<VertexBufferElement> getElement() const { return elements; }
    inline unsigned int getStride() const { return stride; }
};