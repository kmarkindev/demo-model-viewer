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

    m_renderer = new Renderer();
    m_renderer->Init();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

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
    model.SetShader(&shader);
    model.SetScale({ 0.1f, 0.1f, 0.1f });

    camera.SetPerspectiveMatrix(90, g_config.viewportSettings.width, g_config.viewportSettings.height, 0.1f, 100.f);
    camera.SetPosition({30.f, 0.f, 30.f});
    camera.RotateToDirection(model.GetPosition() - camera.GetPosition());

    DirLight dirLight = DirLight();
    dirLight.SetRotationByVector({ 0.5f, 0.5f, 0.f });

    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        m_renderer->Draw(&model, &camera, &dirLight);

        ImGui::Begin("Demo window");
        ImGui::Button("Hello!");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
    }
}

void Application::Shutdown()
{
    CheckInitialization();

    delete m_renderer;
    delete m_loader;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
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
