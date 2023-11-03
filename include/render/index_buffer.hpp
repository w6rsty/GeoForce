#pragma once
#include "glad/glad.h"

class IndexBuffer final {
private:
    unsigned int buffer_id;
public:
    IndexBuffer(const void* data, unsigned int count);
    ~IndexBuffer();
    
    void Bind() const;
    void Unbind() const;
};