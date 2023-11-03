#include "vertex_array.hpp"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &buffer_id);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &buffer_id);
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    glBindVertexArray(buffer_id);
    vb.Bind();

    unsigned int offset = 0;
    const auto& elements = layout.getElement();
    for (unsigned int i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);
        glEnableVertexAttribArray(i);
        offset += element.count * VertexBufferElement::getSize(element.type);
    }
}

void VertexArray::Bind() const {
    glBindVertexArray(buffer_id);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}