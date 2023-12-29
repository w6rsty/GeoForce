#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "camera.hpp"
#include "geo.hpp"
#include "renderer.hpp"
#include "vertex_array.hpp"
#include "index_buffer.hpp"
#include "vertex_buffer.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "anim.hpp"
#include "ui.hpp"
#include "parser.hpp"
#include "analyzer.hpp"
#include "scene.hpp"

#define UINEXT ImGui::SameLine();
#define UIDIVIDER ImGui::Separator();

#define DISPLAY_BUFFER_SIZE 1024
#define EDITOR_BUFFER_SIZE 2048

#define vertexPath "../resources/shader/vertex.glsl"
#define fragPath "../resources/shader/frag.glsl"
#define axisVertexPath "../resources/shader/axis_vertex.glsl"
#define axisFragPath "../resources/shader/axis_frag.glsl"
#define texPath "../resources/img/image.png"
#define fontPath1 "../resources/font/JetBrainsMonoNerdFontMono-Regular.ttf"
#define fontPath2 "../resources/font/JetBrainsMonoNerdFontMono-SemiBold.ttf"

class UI;

enum Mode {
    Numeric = 0,
    Algebra,
    Complex,
};

class Renderer {
private:
    int _precision = 48;
    // 窗口信息
    int _width;
    int _height;
    const char* _name;
    GLFWwindow* _window;

    // 摄像机属性
    Camera* _camera;
    float _aspect;
    bool _first_mouse;
    glm::mat4 _vMat;

    bool _frame_mode;
    float _delta_time;
    float _last_time;
    float _last_x, _last_y;
    std::unordered_map<std::string, Geo*>           _geos;
    std::unordered_map<std::string, VertexArray*>   _vaos;
    std::unordered_map<std::string, VertexBuffer*>  _vbos;
    std::unordered_map<std::string, IndexBuffer*>   _ibos;
    std::unordered_map<std::string, Texture*>       _texs;
    std::unordered_map<std::string, Shader*>        _shaders;
    float _lightColor[3];
    float _lightPos[3];

    char _editor_buffer[EDITOR_BUFFER_SIZE];
    bool _is_editing;
    ShaderType _current_shader_src;

    const float _light_bg = 0.8f;
    const float _dark_bg = 0.2f;
    float _clear_color;

    std::string _display_buffer;
    core::Parser* _parser;
    core::Analyzer* _analyzer;

    bool _axis_mode;
    bool _show_demo;
    bool _modify_presicion;

    friend class UI;
    UI* _ui;

    enum Theme {
        Light =  0,
        Dark,
        Classic,
    };

    int _theme = Theme::Dark;

    Mode _mode = Numeric;

    std::unordered_map<std::string, ImFont*> _fonts;
    std::vector<const char*> _shader_sources = {
        "none",
        vertexPath, 
        fragPath, 
        axisVertexPath,
        axisFragPath};
public:
    Renderer(int w, int h, const char* name);
    ~Renderer();

    bool init();

    void run();

    inline int width() const { return _width; }
    inline int height() const { return _height; }

    void addDisplayChar(const char* str);
    void setDisplayZero();
    void popDisplay();
    inline std::string getDisplay() const { return _display_buffer; }
    void attachParser();
    void executeParser();
private:
    void processInput(GLFWwindow *window);
    void toggle_frame_mode();
    void toggle(bool* value);
};