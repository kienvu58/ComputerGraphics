#pragma once
#include "config.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#define N_KEYS 1024

class App
{
public:
    App();
    ~App();

    void Init();
    void Update(float deltaTime);
    void Render();
    void CleanUp();

    void UpdateKeyState(int key, int action);
    void ProcessMouseMovement(float xoffset, float yoffset);
    void ProcessMouseScroll(float yoffset);

private:
    void DoCameraMovement(float deltaTime);
    bool m_keys[N_KEYS];
    Camera m_camera;
    Shader m_shader;

    std::vector<Model> m_models;
};

