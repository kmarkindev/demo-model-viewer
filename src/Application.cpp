#include "Application.h"

void Application::Init()
{
    if (!glfwInit())
    {
        throw ApplicationInitializationException("GLFW init error");
    }

    if (WINDOW_FULLSCREEN == true)
    {
        m_monitor = glfwGetPrimaryMonitor();
    }

    m_window = glfwCreateWindow(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, DEMO_NAME, m_monitor, nullptr);

    if (!m_window)
    {
        Shutdown();
        throw ApplicationInitializationException("GLFW window creation error");
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGL())
    {
        Shutdown();
        throw ApplicationInitializationException("GLAD init error");
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, KeyCallback);
    glfwSetCursorPosCallback(m_window, CursorPositionCallback);

    m_isInitialized = true;
}

void Application::Start()
{
    CheckInitialization();


    
    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();

        

        glfwSwapBuffers(m_window);
    }
}

void Application::Shutdown()
{
    CheckInitialization();

    glfwTerminate();
}

Application::Application(std::string modelPath)
    : m_modelPath(modelPath)
{
}

void Application::CheckInitialization()
{
    if (!m_isInitialized)
    {
        throw ApplicationNotInitializedException();
    }
}

void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    void* data = glfwGetWindowUserPointer(window);
    Application* app = static_cast<Application*>(data);
}

void Application::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    void* data = glfwGetWindowUserPointer(window);
    Application* app = static_cast<Application*>(data);
}
