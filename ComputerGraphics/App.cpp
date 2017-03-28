#include "App.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\gtc\matrix_transform.hpp>


App::App()
{
    std::fill(m_keys, m_keys + N_KEYS, false);
}


App::~App()
{
}

void App::Init()
{
    m_shader.Init("model.vert", "model.frag");

    Model nanosuit("Resources/Nanosuit/nanosuit.obj");
    m_models.push_back(nanosuit);
}

void App::Update(float deltaTime)
{
    DoCameraMovement(deltaTime);
}

void App::Render()
{
    glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
    m_shader.UseProgram();

    glm::mat4 view = m_camera.GetViewMatrix();
    m_shader.SetUniform("view", view);

    glm::mat4 projection = glm::perspective(m_camera.GetFOV(), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 10000.0f);
    m_shader.SetUniform("projection", projection);


    glm::mat4 world;
    world = glm::translate(world, glm::vec3(0.0f, -5.0f, 0.0f));
    float scale = 0.2f;
    world = glm::scale(world, glm::vec3(scale, scale, scale));
    m_shader.SetUniform("model", world);

    m_models[0].Render(m_shader);

}

void App::CleanUp()
{
    for (Model& model : m_models)
    {
        model.CleanUp();
    }
}

void App::UpdateKeyState(int key, int action)
{
    if (0 <= key && key < N_KEYS)
    {
        if (action == GLFW_PRESS)
        {
            m_keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            m_keys[key] = false;
        }
    }
}

void App::ProcessMouseMovement(float xoffset, float yoffset)
{
    m_camera.ProcessMouseMovement(xoffset, yoffset);
}

void App::ProcessMouseScroll(float yoffset)
{
    m_camera.ProcessMouseScroll(yoffset);
}

void App::DoCameraMovement(float deltaTime)
{
    if (m_keys[GLFW_KEY_W])
    {
        m_camera.Move(CameraMovement::FORWARD, deltaTime);
    }
    if (m_keys[GLFW_KEY_S])
    {
        m_camera.Move(CameraMovement::BACKWARD, deltaTime);
    }
    if (m_keys[GLFW_KEY_A])
    {
        m_camera.Move(CameraMovement::LEFT, deltaTime);
    }
    if (m_keys[GLFW_KEY_D])
    {
        m_camera.Move(CameraMovement::RIGHT, deltaTime);
    }
    if (m_keys[GLFW_KEY_E])
    {
        m_camera.Move(CameraMovement::UP, deltaTime);
    }
    if (m_keys[GLFW_KEY_Q])
    {
        m_camera.Move(CameraMovement::DOWN, deltaTime);
    }
}
