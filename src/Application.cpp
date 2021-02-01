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
    glfwSetCursorPosCallback(m_window, CursorPositionCallback);
    glfwSetMouseButtonCallback(m_window, MouseButtonCallback);
    glfwSetScrollCallback(m_window, ScrollCallback);
    glfwSetWindowSizeCallback(m_window, WindowSizeCallback);

    m_loader = new AssimpLoader(aiProcess_Triangulate 
        | aiProcess_FlipUVs | aiProcess_PreTransformVertices);

    m_renderer = new Renderer();
    m_renderer->Init();
    m_renderer->SetViewport(0, 0, 
        g_config.viewportSettings.width, g_config.viewportSettings.height);

    m_textureLoader = new TextureLoader();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_imguiIo = &ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    m_isInitialized = true;
}

void Application::Start()
{
    CheckInitialization();

    glm::vec3 startCameraPosition = glm::vec3(50.f, 0.f, 0.f);
    glm::vec3 startLightDirection = glm::normalize(glm::vec3(0.5f, 0.f, 0.5f));
    glm::vec3 startModelScale = glm::vec3(0.1f, 0.1f, 0.1f);
    glm::vec3 startModelRotation = glm::vec3(0.f, 0.f, 0.f);
    glm::vec4 startLightColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    float nearPlane = g_config.camera.near;
    float farPlane = g_config.camera.far;
    float fov = g_config.camera.fov;

    Shader shader = Shader(g_config.rootFolder + "/assets/shaders/basic_vertex.vert",
        g_config.rootFolder + "/assets/shaders/basic_fragment.frag");
    shader.LoadAndCompile();

    Material material;
    material.diffuse = m_textureLoader
        ->LoadTexture(g_config.modelFolder + g_config.textures.diffuseName, TextureType::Diffuse);
    material.specular = m_textureLoader
        ->LoadTexture(g_config.modelFolder + g_config.textures.specularName, TextureType::Specular);

    Model model = m_loader->LoadModel(g_config.GetModelPath());
    m_model = &model;
    model.SetShader(&shader);
    model.SetScale(startModelScale);
    model.SetMaterial(material);

    Camera camera = Camera();
    m_camera = &camera;
    camera.SetPerspectiveMatrix(fov, g_config.viewportSettings.width,
        g_config.viewportSettings.height, nearPlane, farPlane);
    camera.SetPosition(startCameraPosition);
    camera.RotateToDirection(model.GetPosition() - camera.GetPosition());

    DirLight dirLight = DirLight();
    dirLight.RotateToDirection(startLightDirection);
    dirLight.color = startLightColor;

    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        m_renderer->Draw(&model, &camera, &dirLight);

        ImGui::Begin("Menu");

        ImGui::BeginChild("Actions", {0, 100});

        if (ImGui::Button("Reset rotation"))
        {
            model.SetRotation(startModelRotation);
        }

        if (ImGui::Button("Reset zoom"))
        {
            model.SetScale(startModelScale);
        }

        ImGui::EndChild();

        ImGui::BeginChild("Light Color");

        float* color = &dirLight.color[0];
        ImGui::ColorPicker4("Color picker", color);
        dirLight.color = glm::vec4(color[0], color[1], color[2], color[3]);

        ImGui::EndChild();

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
    delete m_textureLoader;

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

void Application::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    Application* app = GetInstancePointer(window);

    static float lastx = 0;
    static float lasty = 0;

    float xoffset = xpos - lastx;
    float yoffset = lasty - ypos;

    lastx = xpos;
    lasty = ypos;

    if (!app->m_shouldRotate || app->m_imguiIo->WantCaptureMouse)
    {
        return;
    }

    app->m_model->RotateLocalByVector({ 0, xoffset * g_config.sensivitity, yoffset * g_config.sensivitity });
}

void Application::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    Application* app = GetInstancePointer(window);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        app->m_shouldRotate = true;
    else
        app->m_shouldRotate = false;
}

void Application::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Application* app = GetInstancePointer(window);

    if (app->m_imguiIo->WantCaptureMouse)
    {
        return;
    }

    float step = 1;

    if (yoffset > 0)
    {
        step += g_config.scrollSensivitity;
    }
    else
    {
        step -= g_config.scrollSensivitity;
    }

    app->m_model->Scale({ step, step, step });
}

void Application::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    Application* app = GetInstancePointer(window);

    app->m_camera->SetPerspectiveMatrix(g_config.camera.fov,
        width, height, g_config.camera.near, g_config.camera.far);

    app->m_renderer->SetViewport(0, 0, width, height);
}

Application* Application::GetInstancePointer(GLFWwindow* window)
{
    return static_cast<Application*>(glfwGetWindowUserPointer(window));
}
