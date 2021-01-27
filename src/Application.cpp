#include "Application.h"

void Application::Init()
{
    if (!glfwInit())
    {
        throw ApplicationInitializationException("GLFW init error");
    }

    if (g_config.viewportSettings.fullScreen == true)
    {
        m_monitor = glfwGetPrimaryMonitor();
    }

    m_window = glfwCreateWindow(g_config.viewportSettings.width,
        g_config.viewportSettings.height, g_config.windowTitle.c_str(), m_monitor, nullptr);

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

    m_loader = new AssimpLoader(aiProcess_Triangulate 
        | aiProcess_FlipUVs | aiProcess_PreTransformVertices);

    m_isInitialized = true;
}

void Application::Start()
{
    CheckInitialization();

    Shader shader = Shader(g_config.rootFolder + "/assets/shaders/basic_vertex.vert",
        g_config.rootFolder + "/assets/shaders/basic_fragment.frag");
    shader.LoadAndCompile();

    Camera camera = Camera();
    Model model = m_loader->LoadModel(g_config.GetModelPath());
    model.SetScale({ 0.1f, 0.1f, 0.1f });

    camera.SetPerspectiveMatrix(90, g_config.viewportSettings.width, g_config.viewportSettings.height, 0.1f, 100.f);
    camera.SetPosition({30.f, 0.f, 30.f});
    camera.RotateToDirection(model.GetPosition() - camera.GetPosition());

    auto lightDir = glm::vec3(0.5f, 0.5f, 0.f);

    glClearColor(0.3f, 0.3f, 0.3f, 1.f);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();

        shader.UseProgram();

        shader.SetMat4Uniform("ModelMatrix", model.GetModelMatrix());
        shader.SetMat4Uniform("ViewMatrix", camera.GetViewMatrix());
        shader.SetMat4Uniform("ProjectionMatrix", camera.GetProjectionMatrix());
        shader.SetVec3Uniform("LightDir", lightDir);

        m_renderer->Draw(&model, &shader, &camera);

        glfwSwapBuffers(m_window);
    }
}

void Application::Shutdown()
{
    CheckInitialization();

    delete m_renderer;
    delete m_loader;
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
