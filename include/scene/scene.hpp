#pragma once 

class Scene {
public:
    Scene();
    virtual ~Scene();
    virtual void onUpdate();
    virtual void onRender();
    virtual void onImGuiRender();
};