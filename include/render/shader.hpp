#pragma once

#include "glad/glad.h"
#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

#include "error.hpp"
#include "shader.hpp"

#define SHADER_COUNT 2;

enum ShaderType {
    Vertex,
    Fragment,
    None,
};

class Shader final {
private:
    unsigned int renderer_id;
    std::string* file_paths;
    std::unordered_map<std::string, int> uniform_cache;
private:
    unsigned int compileShader(unsigned int type, const char* source, ShaderType s_type);
    unsigned int createShader();
    int getUniformLocation(const std::string& name);
public:
    Shader(std::string* filePaths);
    ~Shader();
    unsigned int program();
    void Bind() const;
    void Unbind() const;

    static std::string parseShader(const std::string& filePath);
    
    void setUniform1i(const std::string& name, int value);
    void setUniform1f(const std::string& name, float value);
    void setUniform2f(const std::string& name, float v0, float v1);
    void setUniform3f(const std::string& name, float v0, float v1, float v2);
    void setUniformMat4f(const std::string& name, glm::mat4& mat);
};


