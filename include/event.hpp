#pragma once

#include "renderer.hpp"

namespace Enum {
    enum class Key {
        Key_0,
        Key_1,
        Key_2,
        Key_3,
        Key_4,
        Key_5,
        Key_6,
        Key_7,
        Key_8,
        Key_9,
        Key_Add,
        Key_Sub,
        Key_Mul,
        Key_Div,
        Key_Dot,
        Key_Bracket_Left,
        Key_Bracket_Right,
        Key_X,
        Key_Y,
        Key_Z,
        Key_Equal,
        Key_BackSpace,
        Key_AC,
    };
}


inline void handleButton(bool state, Enum::Key key, const char* title, Renderer* rd) {
    if (state) {
        printf("[Key Pressed] %s\n", title);
        switch (key) {
            case Enum::Key::Key_Equal:
                rd->executeParser();
                break;
            case Enum::Key::Key_BackSpace:
                rd->popDisplay();
                break;;
            case Enum::Key::Key_AC:
                rd->setDisplayZero();
                break;
            default:
                rd->addDisplayChar(title);
                break;
        }
    }
}

