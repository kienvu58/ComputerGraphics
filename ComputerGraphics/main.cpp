#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "App.h"


bool InitGLFW();
bool InitGL();

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode);
void CursorPosCallback(GLFWwindow * window, double x, double y);
void ScrollCallback(GLFWwindow * window, double xoffset, double yoffset);

int main()
{
    // init GLFW
    if (!InitGLFW())
    {
        return -1;
    }

    // create window
    //GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, glfwGetPrimaryMonitor(), nullptr);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // set callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    // set GLFW options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // init GL stuffs
    if (!InitGL())
    {
        glfwTerminate();
        return -1;
    }

    // init app
    App app;
    app.Init();
    glfwSetWindowUserPointer(window, &app);

    // main loop
    while (!glfwWindowShouldClose(window))
    {
        static double s_last = 0.0;
        double current = glfwGetTime();
        double deltaTime = current - s_last;
        s_last = current;

        glfwPollEvents();
        app.Update((float)deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        app.Render();
        glfwSwapBuffers(window);

        if (LIMIT_FPS)
        {
            long sleepTime = (long)(1.0 / FPS - deltaTime * 1000.0);
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        }
    }

    app.CleanUp();
    glfwTerminate();
    return 0;
}

bool InitGLFW()
{
    if (glfwInit() == GL_FALSE)
    {
        std::cerr << "[ERROR] cannot initialize GLFW" << std::endl;
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    return true;
}

bool InitGL()
{
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "[ERROR] cannot initialize GLEW" << std::endl;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);
    return true;
}

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    App *pApp = (App*)glfwGetWindowUserPointer(window);
    pApp->UpdateKeyState(key, action);
}

void CursorPosCallback(GLFWwindow * window, double x, double y)
{
    static bool s_firstMouse = true;
    static float s_lastX = WIDTH / 2;
    static float s_lastY = HEIGHT / 2;

    if (s_firstMouse)
    {
        s_lastX = (float)x;
        s_lastY = (float)y;
        s_firstMouse = false;
    }

    float xoffset = (float)x - s_lastX;
    float yoffset = s_lastY - (float)y;

    s_lastX = (float)x;
    s_lastY = (float)y;

    App *pApp = (App*)glfwGetWindowUserPointer(window);
    pApp->ProcessMouseMovement(xoffset, yoffset);
}

void ScrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
    App *pApp = (App*)glfwGetWindowUserPointer(window);
    pApp->ProcessMouseScroll((float)yoffset);
}
