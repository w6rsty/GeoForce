#pragma once

#include "renderer.hpp"

class Renderer;

class UI {
private:
    Renderer* _rd;
public:
    UI(Renderer* rd);

    void imguiInit();
    void imguiLayout();
    void imguiMainTabBar();
    void imguiOperationPanel();    
    void imguiGLSLEditor();
    void initEditor();

};