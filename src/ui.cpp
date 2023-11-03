#include "renderer.hpp"

#include "event.hpp"
#include "glsl_loader.hpp"

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
            unsigned int size = sizeof(char) * EDITOR_BUFFER_SIZE;
            if (ImGui::MenuItem("Load Vertex Shader")) {
                loadShaderSource("../resources/shader/vertex.glsl", _editor_buffer, size);
                _current_shader_src = ShaderType::Vertex;
            }
            if (ImGui::MenuItem("Load Fragment Shader")) {
                loadShaderSource("../resources/shader/frag.glsl", _editor_buffer, size);
                _current_shader_src = ShaderType::Fragment;
            }

            if (ImGui::MenuItem("Save Shader File")) {

            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}