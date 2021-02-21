#include "Application.h"

void Application::Init()
{
    if (!glfwInit())
    {
        throw ApplicationInitializationException("GLFW init error");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);

    if (g_config.viewport.fullScreen == true)
    {
        m_monitor = glfwGetPrimaryMonitor();
    }

    m_window = glfwCreateWindow(g_config.viewport.width,
        g_config.viewport.height, g_config.windowTitle.c_str(), m_monitor, nullptr);

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
        g_config.viewport.width, g_config.viewport.height);

    m_textureLoader = new TextureLoader();

    InitImgui();

    m_isInitialized = true;
}

void Application::Start()
{
    CheckInitialization();

    SetupModel();
    SetupCamera();
    SetupLight();

    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();

        m_renderer->Draw(m_model, m_camera, m_light);

        DrawImguiUi();

        glfwSwapBuffers(m_window);
    }
}

void Application::Shutdown()
{
    CheckInitialization();

    delete m_model;
    delete m_camera;
    delete m_light;
    delete m_renderer;
    delete m_loader;
    delete m_textureLoader;

    ShutdownImgui();

    glfwDestroyWindow(m_window);
    glfwTerminate();

    m_isInitialized = false;
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

void Application::SetupModel()
{
    Shader* shader = new Shader(g_config.rootFolder + "/assets/shaders/basic_vertex.vert",
        g_config.rootFolder + "/assets/shaders/basic_fragment.frag");
    shader->LoadAndCompile();

    Material* material = new Material();
    material->diffuse = m_textureLoader
        ->LoadTexture(g_config.modelFolder + g_config.textures.diffuseName, TextureType::Diffuse);
    material->specular = m_textureLoader
        ->LoadTexture(g_config.modelFolder + g_config.textures.specularName, TextureType::Specular);

    Model* model = m_loader->LoadModel(g_config.GetModelPath());
    model->SetShader(shader);
    model->SetScale(m_startModelScale);
    model->SetMaterial(material);

    m_model = model;
}

void Application::SetupCamera()
{
    Camera* camera = new Camera();
    camera->SetPerspectiveMatrix(g_config.camera.fov, g_config.viewport.width,
        g_config.viewport.height, g_config.camera.near, g_config.camera.far);
    camera->SetPosition(m_startCameraPosition);
    camera->RotateToDirection(m_model->GetPosition() - camera->GetPosition());

    m_camera = camera;
}

void Application::SetupLight()
{
    DirLight* dirLight = new DirLight();
    dirLight->RotateToDirection(m_startLightDirection);
    dirLight->color = m_startLightColor;

    m_light = dirLight;
}

void Application::InitImgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_imguiIo = &ImGui::GetIO();

    m_imguiIo->IniFilename = nullptr;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void Application::ShutdownImgui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Application::StartImguiFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Application::EndImgueFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::DrawImguiUi()
{
    StartImguiFrame();

    ImGui::SetNextWindowBgAlpha(m_menuAlpha);

    ImGui::Begin("Menu");

    if(ImGui::CollapsingHeader("Actions"))
    {
        if (ImGui::Button("Reset rotation"))
        {
            m_model->SetRotation(glm::quat(1.f, 0.f, 0.f, 0.f));
        }

        ImGui::SameLine();

        if (ImGui::Button("Reset zoom"))
        {
            m_model->SetScale(m_startModelScale);
        }
    }

    if(ImGui::CollapsingHeader("Anti Aliasing"))
    {
        bool enableAA = m_renderer->IsAntiAliasingEnabled();
        if (ImGui::Checkbox("Enabled##AA_Enabled", &enableAA))
        {
            m_renderer->ToggleAntiAliasing(enableAA);
        }
    }

    if (ImGui::CollapsingHeader("Face culling"))
    {
        bool enableFaceCulling = m_renderer->IsFaceCullingEnabled();
        if (ImGui::Checkbox("Enabled##FC_enabled", &enableFaceCulling)) 
        {
            m_renderer->ToggleFaceCulling(enableFaceCulling);
        }

        bool radioChanged = false;
        int value = (int)m_renderer->GetFaceCullingMode();
        radioChanged |= ImGui::RadioButton("Front", &value, 0);
        ImGui::SameLine();
        radioChanged |= ImGui::RadioButton("Back", &value, 1);
        ImGui::SameLine();
        radioChanged |= ImGui::RadioButton("Front and back", &value, 2);

        if(radioChanged)
        {
            m_renderer->SetFaceCullingMode((CullingMode)value);
        }
    }

    if (ImGui::CollapsingHeader("Light"))
    {
        float* color = &m_light->color[0];
        ImGui::ColorPicker4("Light color", color);
        m_light->color = glm::vec4(color[0], color[1], color[2], color[3]);
    }

    if(ImGui::CollapsingHeader("Menu settings"))
    {
        ImGui::SliderFloat("Menu Opacity", &m_menuAlpha, 0.f, 1.f);
    }

    ImGui::End();

    EndImgueFrame();
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
