#pragma once
#include "glad/glad.h"

class VertexBuffer final {
private:
    unsigned int buffer_id;
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void update(const void* data, unsigned int size);
    void Bind() const;
    void Unbind() const;
};