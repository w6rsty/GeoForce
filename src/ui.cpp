#include "ui.hpp"

#include "imgui.h"

#include "event.hpp"
#include "glsl_loader.hpp"
#include "renderer.hpp"
#include "shader.hpp"

UI::UI(Renderer* rd) {
    _rd = rd;
}

void UI::imguiInit() {
    printf("[Dear ImGui] %s\n", ImGui::GetVersion());
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    _rd->_fonts["global"] = io.Fonts->AddFontFromFileTTF(fontPath1, 30.0f);
    io.FontDefault = _rd->_fonts["global"];
    _rd->_fonts["display"] = io.Fonts->AddFontFromFileTTF(fontPath2, 60.0f);

    switch (_rd->_theme) {
        case 0: {
            ImGui::StyleColorsLight();
            _rd->_clear_color = _rd->_light_bg;
            break;
        }
        case 1: {
            ImGui::StyleColorsDark();
            _rd->_clear_color = _rd->_dark_bg;
            break;
        }
        case 2: {
            ImGui::StyleColorsClassic();
            _rd->_clear_color = _rd->_dark_bg;
            break;
        }
    }

    ImGui_ImplGlfw_InitForOpenGL( _rd->_window, true);
    const char* glsl_version = "#version 460";
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void UI::imguiLayout() {
    imguiMainTabBar();
    imguiOperationPanel();
    imguiGLSLEditor();
    if ( _rd->_show_demo) ImGui::ShowDemoWindow();
}

void UI::imguiOperationPanel() {
    ImGui::SetNextWindowPos(ImVec2(0, _rd->_height / 2.0));
    ImGui::SetNextWindowSize(ImVec2(_rd->_width / 2.0, _rd->_height / 2.0));
    ImGui::Begin("Operation", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if ( _rd->_modify_presicion) {
        ImGui::SliderInt("Precsion", &_rd->_precision, 2, 48);
    }
    ImGui::PushFont(_rd->_fonts["display"]);
    ImGui::Text("%s", _rd->_display_buffer.c_str());
    ImGui::PopFont();

    UIDIVIDER

    ImVec2 bt_size(120, 80);
    if (_rd->_mode <= Mode::Complex) {
        handleButton(ImGui::Button("7", bt_size), Enum::Key::Key_7, "7", _rd); UINEXT
        handleButton(ImGui::Button("8", bt_size), Enum::Key::Key_8, "8", _rd); UINEXT
        handleButton(ImGui::Button("9", bt_size), Enum::Key::Key_9, "9", _rd); UINEXT
        ImGui::Spacing(); UINEXT
        handleButton(ImGui::Button("<--", bt_size), Enum::Key::Key_BackSpace, "BackSpace", _rd); UINEXT
        handleButton(ImGui::Button("AC", bt_size), Enum::Key::Key_AC, "AC", _rd); UINEXT
        if(ImGui::Button("DEBUG", bt_size)) {
            std::cout << _rd->getDisplay() << std::endl;
        }
    }
    UIDIVIDER

    if (_rd->_mode <= Mode::Complex) {
        handleButton(ImGui::Button("4", bt_size), Enum::Key::Key_4, "4",_rd); UINEXT
        handleButton(ImGui::Button("5", bt_size), Enum::Key::Key_5, "5", _rd); UINEXT
        handleButton(ImGui::Button("6", bt_size), Enum::Key::Key_6, "6", _rd); UINEXT
        ImGui::Spacing(); UINEXT
        handleButton(ImGui::Button("+", bt_size), Enum::Key::Key_Add, "+", _rd); UINEXT
        handleButton(ImGui::Button("-", bt_size), Enum::Key::Key_Sub, "-", _rd);
    }
    if (_rd->_mode == Mode::Algebra) {
        UINEXT
        handleButton(ImGui::Button("x", bt_size), Enum::Key::Key_X, "x", _rd);
    }

    UIDIVIDER

    if (_rd->_mode <= Mode::Complex) {
        handleButton(ImGui::Button("1", bt_size), Enum::Key::Key_1, "1", _rd); UINEXT
        handleButton(ImGui::Button("2", bt_size), Enum::Key::Key_2, "2", _rd); UINEXT
        handleButton(ImGui::Button("3", bt_size), Enum::Key::Key_3, "3", _rd); UINEXT
        ImGui::Spacing(); UINEXT
        handleButton(ImGui::Button("X", bt_size), Enum::Key::Key_Mul, "*", _rd); UINEXT
        handleButton(ImGui::Button("/", bt_size), Enum::Key::Key_Div, "/", _rd);
    }
    if (_rd->_mode == Mode::Algebra) {
        UINEXT
        handleButton(ImGui::Button("y", bt_size), Enum::Key::Key_Y, "y", _rd); 
    }

    UIDIVIDER

    if (_rd->_mode <= Mode::Complex) {
        handleButton(ImGui::Button("0", bt_size), Enum::Key::Key_0, "0", _rd); UINEXT
        handleButton(ImGui::Button(".", bt_size), Enum::Key::Key_Dot, ".", _rd); UINEXT
        handleButton(ImGui::Button("=", bt_size), Enum::Key::Key_Equal, "=", _rd); UINEXT
        ImGui::Spacing(); UINEXT
        handleButton(ImGui::Button("(", bt_size), Enum::Key::Key_Bracket_Left, "(", _rd); UINEXT
        handleButton(ImGui::Button(")", bt_size), Enum::Key::Key_Bracket_Right, ")", _rd);
    }
    if (_rd->_mode == Mode::Algebra) {
        UINEXT
        handleButton(ImGui::Button("z", bt_size), Enum::Key::Key_Z, "z", _rd); 
    }
    ImGui::End();
}

void UI::initEditor() {
    _rd->_is_editing = false;
    _rd->_current_shader_src = ShaderType::None;
    memset(_rd->_editor_buffer, 0, sizeof(char) *  EDITOR_BUFFER_SIZE);
    _rd->_editor_buffer[0] = '\0';
}

void UI::imguiGLSLEditor() {
    float tab_height = ImGui::GetTextLineHeightWithSpacing();
    ImGui::SetNextWindowPos(ImVec2((float)_rd->_width / 2.0, tab_height));
    ImGui::SetNextWindowSize(ImVec2((float)_rd->_width / 2.0, _rd->_height-tab_height));
    ImGui::Begin("GLSL Editor", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    static int item_current = 0;
    ImGui::Combo("File", &item_current, _rd->_shader_sources.data(), _rd->_shader_sources.size()); UINEXT
    if (item_current != 0) {
        _rd->_is_editing = true;
        unsigned int size = sizeof(char) * EDITOR_BUFFER_SIZE;
        loadShaderSource(_rd->_shader_sources[item_current], _rd->_editor_buffer, size);
        if (ImGui::Button("Save")) {
            printf("%s\n", _rd->_editor_buffer);
            writeShaderSource(_rd->_shader_sources[item_current], _rd->_editor_buffer);
        }

        ImGui::InputTextMultiline(
            "##source",
            _rd->_editor_buffer,
            sizeof(char) * EDITOR_BUFFER_SIZE,
            ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 30)
        );
    }
    
    if (ImGui::IsItemActive()) {
       _rd-> _is_editing = true;
    } else {
        _rd->_is_editing = false;
    }

    ImGui::End();
}

void UI::imguiMainTabBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Menu")) {
            if (ImGui::MenuItem("quit")) {
                glfwSetWindowShouldClose(_rd->_window, true);
            }
            if (ImGui::BeginMenu("Preference")) {
                if (ImGui::BeginMenu("Theme")) {
                    if (ImGui::MenuItem("Light")) {
                        ImGui::StyleColorsLight();
                        _rd->_clear_color = _rd->_light_bg;
                    }
                    if (ImGui::MenuItem("Dark")) {
                        ImGui::StyleColorsDark();
                        _rd->_clear_color = _rd->_dark_bg;
                    }
                    if (ImGui::MenuItem("Classic")) {
                        ImGui::StyleColorsClassic();
                        _rd->_clear_color = _rd->_dark_bg;
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Frame mode", _rd->_frame_mode ? "ON" : "OFF")) {
                _rd->toggle_frame_mode();
            }
            if (ImGui::MenuItem("Axis mode", _rd->_axis_mode ? "ON" : "OFF")) {
                _rd->toggle(&_rd->_axis_mode);
            }
            if (ImGui::MenuItem("Demo")) {
                _rd->toggle(&_rd->_show_demo);
            }

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Mode")) {
            if (ImGui::MenuItem("Numeric", _rd->_mode == Mode::Numeric ? "*" : "")) {
                _rd->_mode = Mode::Numeric;
            }
            if (ImGui::MenuItem("Algera", _rd->_mode == Mode::Algebra ? "*" : "")) {
                _rd->_mode = Mode::Algebra;
            }
            if (ImGui::MenuItem("Complex", _rd->_mode == Mode::Complex ? "*" : "")) {
                _rd->_mode = Mode::Complex;
            }
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Modify")) {
            if (ImGui::MenuItem("Presision")) {
                _rd->toggle(&_rd->_modify_presicion);
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}