#pragma once 

class Scene {
public:
    Scene();
    virtual ~Scene();
    virtual void onUpdate() = 0;
    virtual void onRender() = 0;
    virtual void onImGuiRender();
};