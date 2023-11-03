#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cstdio>

namespace Enum {
    enum class Key {
        Key_1,
        Key_2,
        Key_3,
        Key_4,
        Key_5,
        Key_6,
        Key_7,
        Key_8,
        Key_9,
        Key_0,
        Key_Add,
        Key_Sub,
        Key_Mul,
        Key_Div,
        Key_Equal,
        Key_Dot,
        Key_BackSpace,
        Key_AC,
        Key_Bracket_Left,
        Key_Bracket_Right,
        Key_X,
        Key_Y,
        Key_Z,
    };
}


inline void handleButton(bool state, Enum::Key key, const char* title) {
    if (state) {
        printf("[Key Pressed] %s\n", title);
    }
}

