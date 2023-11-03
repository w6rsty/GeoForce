#include "renderer.hpp"

#include "event.hpp"

const char* vertexPath = "../resources/shader/vertex.glsl";
const char* fragPath = "../resources/shader/frag.glsl";
const char* texPath = "../resources/img/image.png";
const char* fontPath1 = "../resources/font/JetBrainsMonoNerdFontMono-Regular.ttf";
const char* fontPath2 = "../resources/font/JetBrainsMonoNerdFontMono-SemiBold.ttf";

Renderer::Renderer(int w, int h, const char* name)
    : _width(w), _height(h), _name(name)
{
    _aspect = (float)_width / (float)_height;
    _frame_mode = false;

    _first_mouse = false;
    _delta_time = 0.0f;
    _last_time = 0.0f;
    _last_x = _width / 2.0f;
    _last_y = _height / 2.0f;

    _camera = new Camera(glm::vec3(0.0, 0.0, 3.0f));
}

Renderer::~Renderer() {
    _geos.clear();
    _vaos.clear();
    _vbos.clear();
    _ibos.clear();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    if (_window) {
        glfwDestroyWindow(_window);
    }
    if (_camera) {
        delete  _camera;
    }
    glfwTerminate();
}

bool Renderer::init() {
    if (!glfwInit()) {
        printf("\x1b[31;1m Failed to initialize GLFW\n\x1b[0m\n");
        return false;
    }

    glfwWindowHint(GLFW_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    _window = glfwCreateWindow(_width, _height, _name, NULL, NULL);
    if (_window == nullptr) {
        printf("\x1b[31;1mFaild to create GLFW window\n\x1b[0m\n");
        return false;
    } else {
        printf("\x1b[32;1mSuccessfully create window '%s' (%d,%d)\n\x1b[0m", _name, _width, _height);
    }
    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);


    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("\x1b[31;1mFailed to initialize OpenGL context\n\x1b[0m\n");
        return false;
    }
    
    imguiInit();

    Geo* cube = new Sphere(96);

    _geos["Sphere"] = cube;
    _vaos["Sphere"] = new VertexArray();
    _vbos["Sphere"] = new VertexBuffer(cube->getVertices(), cube->getSize());
    _ibos["Sphere"] = new IndexBuffer(cube->getIndices(), cube->getCount());

    VertexBufferLayout layout;
    layout.push_float(3);
    layout.push_float(2);
    layout.push_float(3);
    _vaos["Sphere"]->addBuffer(*_vbos["Sphere"], layout);
    
    unsigned char tex_data[640 * 640 * 3];
    createCheckboardTexture(tex_data, 640, 640, 16);
    Texture* tex = new Texture(640, 640, tex_data);
    
    _texs["Sphere"] = tex;
    _texs["img"] = new Texture(texPath);
     
    std::string paths[] = {
        vertexPath, fragPath
    };
    _shader = new Shader(paths);

    _lightColor[0] = 1.0f;
    _lightColor[1] = 1.0f;
    _lightColor[2] = 1.0f;
    _lightPos[0] = 3.0f;
    _lightPos[1] = 5.0f;
    _lightPos[2] = 4.0f;

    _is_editing = false;

    _current_shader_src = ShaderType::None;
    memset(_editor_buffer, 0, sizeof(char) *  EDITOR_BUFFER_SIZE);
    _editor_buffer[0] = '\0';
    return true;
}

void Renderer::run() {
    glViewport(0, _height / 2.0, _width / 2.0, _height / 2.0);
    while (!glfwWindowShouldClose(_window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        _delta_time = currentFrame - _last_time;
        _last_time= currentFrame;

        processInput(_window);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        imguiLayout();

        glClearColor(_clear_color, _clear_color, _clear_color, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        _shader->Bind();
        _ibos["Sphere"]->Bind();
        _texs["img"]->Bind(0);
        _vaos["Sphere"]->Bind();

        _shader->setUniform1i("samp", 0);
        _vMat = glm::perspective(glm::radians(_camera->Zoom), _aspect, 0.1f, 1000.0f);
        _shader->setUniformMat4f("proj_matrix", _vMat);
        auto view = _camera->GetViewMatrix();
        _shader->setUniformMat4f("view_matrix", view);

        _shader->setUniform3f("lightColor", _lightColor[0], _lightColor[1], _lightColor[2]);
        _shader->setUniform3f("lightPos", _lightPos[0], _lightPos[1], _lightPos[2]);
        _shader->setUniform3f("viewPos", _camera->Position.x, _camera->Position.y, _camera->Position.z);

        glm::mat4 mMat = glm::scale(glm::mat4(1.0f), glm::vec3(0.8, 0.8, 0.8));
        mMat *= glm::rotate(glm::mat4(1.0f), 0.5f * (float)glfwGetTime(), glm::vec3(0.0, 1.0, 0.0));
        _shader->setUniformMat4f("model_matrix", mMat);

        glDrawElements(GL_TRIANGLES, _geos["Sphere"]->getCount(), GL_UNSIGNED_INT, 0);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(_window);
        glfwPollEvents();   
    }
}

void Renderer::imguiInit() {
    printf("[Dear ImGui] %s\n", ImGui::GetVersion());
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    _fonts["global"] = io.Fonts->AddFontFromFileTTF(fontPath1, 30.0f);
    io.FontDefault = _fonts["global"];
    _fonts["display"] = io.Fonts->AddFontFromFileTTF(fontPath2, 60.0f);

    switch (_theme) {
        case 0: {
            ImGui::StyleColorsLight();
            _clear_color = _light_bg;
            break;
        }
        case 1: {
            ImGui::StyleColorsDark();
            _clear_color = _dark_bg;
            break;
        }
        case 2: {
            ImGui::StyleColorsClassic();
            _clear_color = _dark_bg;
            break;
        }
    }

    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    const char* glsl_version = "#version 460";
    ImGui_ImplOpenGL3_Init(glsl_version);
}


void Renderer::imguiLayout() {
    imguiMainTabBar();
    imguiOperationPanel();
    imguiGLSLEditor();
}

void Renderer::processInput(GLFWwindow *window)
{
    if (!_is_editing) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            _camera->ProcessKeyboard(FORWARD, _delta_time);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            _camera->ProcessKeyboard(BACKWARD, _delta_time);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            _camera->ProcessKeyboard(LEFT, _delta_time);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            _camera->ProcessKeyboard(RIGHT, _delta_time);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            _camera->ProcessKeyboard(UP, _delta_time);
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            _camera->ProcessKeyboard(DOWN, _delta_time);
    }
}


void Renderer::toggle_frame_mode() {
    _frame_mode = !_frame_mode;
    if (_frame_mode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void Renderer::imguiOperationPanel() {
    ImGui::SetNextWindowPos(ImVec2(0, _height / 2.0));
    ImGui::SetNextWindowSize(ImVec2(_width / 2.0, -FLT_MIN));
    ImGui::Begin("Operation", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::PushFont(_fonts["display"]);
    ImGui::Text("1141514");
    ImGui::PopFont();

    UIDIVIDER

    ImVec2 bt_size(120, 80);
    handleButton(ImGui::Button("7", bt_size), Enum::Key::Key_7, "7"); UINEXT
    handleButton(ImGui::Button("8", bt_size), Enum::Key::Key_8, "8"); UINEXT
    handleButton(ImGui::Button("9", bt_size), Enum::Key::Key_9, "9"); UINEXT
    ImGui::Spacing(); UINEXT
    handleButton(ImGui::Button("<--", bt_size), Enum::Key::Key_BackSpace, "BackSpace"); UINEXT
    handleButton(ImGui::Button("AC", bt_size), Enum::Key::Key_AC, "AC");
    UIDIVIDER

    handleButton(ImGui::Button("4", bt_size), Enum::Key::Key_4, "4"); UINEXT
    handleButton(ImGui::Button("5", bt_size), Enum::Key::Key_5, "5"); UINEXT
    handleButton(ImGui::Button("6", bt_size), Enum::Key::Key_6, "6"); UINEXT
    ImGui::Spacing(); UINEXT
    handleButton(ImGui::Button("+", bt_size), Enum::Key::Key_Add, "Add"); UINEXT
    handleButton(ImGui::Button("-", bt_size), Enum::Key::Key_Sub, "Sub"); UINEXT
    handleButton(ImGui::Button("x", bt_size), Enum::Key::Key_X, "x");

    UIDIVIDER

    handleButton(ImGui::Button("1", bt_size), Enum::Key::Key_1, "1"); UINEXT
    handleButton(ImGui::Button("2", bt_size), Enum::Key::Key_2, "2"); UINEXT
    handleButton(ImGui::Button("3", bt_size), Enum::Key::Key_3, "3"); UINEXT
    ImGui::Spacing(); UINEXT
    handleButton(ImGui::Button("X", bt_size), Enum::Key::Key_Mul, "Mul"); UINEXT
    handleButton(ImGui::Button("/", bt_size), Enum::Key::Key_Div, "Div"); UINEXT
    handleButton(ImGui::Button("y", bt_size), Enum::Key::Key_Y, "y"); 

    UIDIVIDER
    handleButton(ImGui::Button("0", bt_size), Enum::Key::Key_0, "0"); UINEXT
    handleButton(ImGui::Button(".", bt_size), Enum::Key::Key_Dot, "Dot"); UINEXT
    handleButton(ImGui::Button("=", bt_size), Enum::Key::Key_Equal, "Equal"); UINEXT
    ImGui::Spacing(); UINEXT
    handleButton(ImGui::Button("(", bt_size), Enum::Key::Key_Bracket_Left, "("); UINEXT
    handleButton(ImGui::Button(")", bt_size), Enum::Key::Key_Bracket_Right, ")"); UINEXT
    handleButton(ImGui::Button("z", bt_size), Enum::Key::Key_Z, "z"); 

    ImGui::End();
}


void Renderer::imguiGLSLEditor() {
    float tab_height = ImGui::GetTextLineHeightWithSpacing();
    ImGui::SetNextWindowPos(ImVec2(800, tab_height));
    ImGui::SetNextWindowSize(ImVec2(800, _height-tab_height));
    ImGui::Begin("GLSL Editor", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::InputTextMultiline(
        "##source",
        _editor_buffer,
        sizeof(char) * EDITOR_BUFFER_SIZE,
        ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 30)
    );
    
    if (ImGui::IsItemActive()) {
        _is_editing = true;
    } else {
        _is_editing = false;
    }

    ImGui::End();
}

void Renderer::imguiMainTabBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Menu")) {
            if (ImGui::MenuItem("quit")) {
                glfwSetWindowShouldClose(_window, true);
            }
            if (ImGui::BeginMenu("Preference")) {
                if (ImGui::BeginMenu("Theme")) {
                    if (ImGui::MenuItem("Light")) {
                        ImGui::StyleColorsLight();
                        _clear_color = _light_bg;
                    }
                    if (ImGui::MenuItem("Dark")) {
                        ImGui::StyleColorsDark();
                        _clear_color = _dark_bg;
                    }
                    if (ImGui::MenuItem("Classic")) {
                        ImGui::StyleColorsClassic();
                        _clear_color = _dark_bg;
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Frame mode")) {
                toggle_frame_mode();
            }
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Editor")) {
            if (ImGui::MenuItem("Load Vertex Shader")) {
                loadShaderSource("../resources/shader/vertex.glsl", _editor_buffer);
                _current_shader_src = ShaderType::Vertex;
            }
            if (ImGui::MenuItem("Load Fragment Shader")) {
                loadShaderSource("../resources/shader/frag.glsl", _editor_buffer);
                _current_shader_src = ShaderType::Fragment;
            }

            if (ImGui::MenuItem("Save Shader File")) {

            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}