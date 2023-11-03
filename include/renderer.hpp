#pragma once
#include "pch.hpp"

#include "camera.hpp"
#include "geo.hpp"
#include "renderer.hpp"
#include "vertex_array.hpp"
#include "index_buffer.hpp"
#include "vertex_buffer.hpp"
#include "texture.hpp"
#include "shader.hpp"

#define UINEXT ImGui::SameLine();
#define UIDIVIDER ImGui::Separator();

#define DISPLAY_BUFFER_SIZE 1024
#define EDITOR_BUFFER_SIZE 2048

#define vertexPath "../resources/shader/vertex.glsl"
#define fragPath "../resources/shader/frag.glsl"
#define texPath "../resources/img/image.png"
#define fontPath1 "../resources/font/JetBrainsMonoNerdFontMono-Regular.ttf"
#define fontPath2 "../resources/font/JetBrainsMonoNerdFontMono-SemiBold.ttf"

class Renderer {
private:
    int _width;
    int _height;
    const char* _name;
    GLFWwindow* _window;

    float _aspect;
    bool _frame_mode;
    glm::mat4 _vMat;

    Camera* _camera;
    bool _first_mouse;
    float _delta_time;
    float _last_time;
    float _last_x, _last_y;
    std::unordered_map<std::string, Geo*> _geos;
    std::unordered_map<std::string, VertexArray*> _vaos;
    std::unordered_map<std::string, VertexBuffer*> _vbos;
    std::unordered_map<std::string, IndexBuffer*> _ibos;
    std::unordered_map<std::string, Texture*> _texs;

    Shader* _shader;
    float _lightColor[3];
    float _lightPos[3];

    char _editor_buffer[EDITOR_BUFFER_SIZE];
    bool _is_editing;
    ShaderType _current_shader_src;

    const float _light_bg = 0.8f;
    const float _dark_bg = 0.2f;
    float _clear_color;

    char _display_buffer[DISPLAY_BUFFER_SIZE];

    enum Theme {
        Light =  0,
        Dark,
        Classic,
    };

    int _theme = Theme::Dark;
    std::unordered_map<std::string, ImFont*> _fonts;
public:
    Renderer(int w, int h, const char* name);
    ~Renderer();

    bool init();

    void run();

    void imguiInit();
    void imguiLayout();
    void imguiMainTabBar();
    void imguiOperationPanel();    
    void imguiGLSLEditor();

    void processInput(GLFWwindow *window);

    void toggle_frame_mode();

    inline int width() const { return _width; }
    inline int height() const { return _height; }
};