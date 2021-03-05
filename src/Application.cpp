#include "Application.h"

void Application::Init(int argc, char* argv[])
{
    if (!glfwInit())
    {
        throw ApplicationInitializationException("GLFW init error");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);

    if (m_isFullscreenOnStart == true)
    {
        m_monitor = glfwGetPrimaryMonitor();
    }

    m_window = glfwCreateWindow(m_startWidth,
        m_startHeight, m_startWindowTitle.c_str(), m_monitor, nullptr);

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

    std::string rootFolder = std::filesystem::path(argv[0]).parent_path().string();
    m_assetsManager = new AssetsManager(rootFolder);

    m_loader = new AssimpLoader(
        aiProcess_Triangulate 
        | aiProcess_FlipUVs
        | aiProcess_PreTransformVertices
        | aiProcess_CalcTangentSpace
    );

    m_renderer = new Renderer();
    m_renderer->Init();
    m_renderer->SetViewport(0, 0, 
        m_startWidth, m_startHeight);

    m_textureLoader = new TextureLoader();

    InitImgui();

    m_isInitialized = true;
}

void Application::Start()
{
    CheckInitialization();

    SetupShader();
    SetupCamera();
    SetupLight();

    LoadSkybox({
        m_assetsManager->GetAssetPath({"skybox", "SkyBox020001.tif"}),
        m_assetsManager->GetAssetPath({"skybox", "SkyBox020002.tif"}),
        m_assetsManager->GetAssetPath({"skybox", "SkyBox020003.tif"}),
        m_assetsManager->GetAssetPath({"skybox", "SkyBox020004.tif"}),
        m_assetsManager->GetAssetPath({"skybox", "SkyBox020005.tif"}),
        m_assetsManager->GetAssetPath({"skybox", "SkyBox020006.tif"})
    });
    LoadModel(m_assetsManager->GetAssetPath({"models", "backpack", "Survival_BackPack_2.fbx"}));
    LoadTexture(m_assetsManager->GetAssetPath({"models", "backpack","1001_albedo.jpg"}), TextureType::Diffuse);
    LoadTexture(m_assetsManager->GetAssetPath({"models", "backpack", "1001_metallic.jpg"}), TextureType::Specular);
    LoadTexture(m_assetsManager->GetAssetPath({"models", "backpack", "1001_normal.png"}), TextureType::Normal);
    
    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();

        m_renderer->Draw(m_model, m_camera, m_light, m_skybox);

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

void Application::CheckInitialization()
{
    if (!m_isInitialized)
    {
        throw ApplicationNotInitializedException();
    }
}

void Application::SetupShader() 
{
    m_shader = new Shader(m_assetsManager->GetAssetPath({"shaders", "basic_vertex.vert"}),
        m_assetsManager->GetAssetPath({"shaders", "basic_fragment.frag"}));
    m_shader->LoadAndCompile();

    m_skyboxShader = new Shader(m_assetsManager->GetAssetPath({"shaders", "skybox.vert"}),
        m_assetsManager->GetAssetPath({"shaders", "skybox.frag"}));
    m_skyboxShader->LoadAndCompile();
}

void Application::LoadModel(std::string path)
{
    if(m_model)
    {
        m_model->Unload();
        delete m_model;
    }

    m_model = m_loader->LoadModel(path);
    m_model->SetShader(m_shader);
    m_model->SetScale(m_startModelScale);
    m_model->SetMaterial(new Material());
}

void Application::LoadTexture(std::string path, TextureType type) 
{
    if(!m_model)
        throw CannotSetTexture();
    
    Texture texture = m_textureLoader
        ->LoadTexture(path, type);

    Material* material = m_model->GetMaterial();
    material->ReplaceTexture(texture, type);
}

void Application::LoadSkybox(std::vector<std::string> paths) 
{
    if(m_skybox)
    {
        m_skybox->Unload();
    }

    Cubemap cubemap = m_textureLoader->LoadCubemap(paths);

    m_skybox = new Skybox(cubemap, m_skyboxShader);
}

void Application::SetupCamera()
{
    glm::vec3 modelPosition = m_model ? GetModelPosition() : glm::vec3(0,0,0);

    m_camera = new Camera();
    m_camera->SetPerspectiveMatrix(m_startFov, m_startWidth,
        m_startHeight, m_startNear, m_startFar);
    m_camera->SetPosition(m_startCameraPosition);
    m_camera->RotateToDirection(GetModelPosition() - m_camera->GetPosition());
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
    ImGui::SetNextWindowSize({300, 500}, ImGuiWindowFlags_NoResize);
    ImGui::SetNextWindowPos({1,1});

    ImGui::Begin("Menu");

    if(ImGui::CollapsingHeader("Assets"))
    {
        if(ImGui::Button("Load model"))
            ImGuiFileDialog::Instance()
                ->OpenDialog("ChooseModel", "Choose model", ".fbx,.obj", ".");

        if(m_model){

            /* diffuse */
            ImGui::Image((void*)(intptr_t)m_model->GetMaterial()->diffuse.id, {128, 128});

            ImGui::SameLine();

            if(ImGui::Button("Load diffuse"))
                ImGuiFileDialog::Instance()
                    ->OpenDialog("ChooseDiffuse", "Choose diffuse texture", ".png,.jpg,.jpeg", ".");

            /* specular */
            ImGui::Image((void*)(intptr_t)m_model->GetMaterial()->specular.id, {128, 128});

            ImGui::SameLine();

            if(ImGui::Button("Load specular"))
                ImGuiFileDialog::Instance()
                    ->OpenDialog("ChooseSpecular", "Choose specular texture", ".png,.jpg,.jpeg", ".");

            /* normal */
            ImGui::Image((void*)(intptr_t)m_model->GetMaterial()->normal.id, {128, 128});

            ImGui::SameLine();

            if(ImGui::Button("Load normal"))
                ImGuiFileDialog::Instance()
                    ->OpenDialog("ChooseNormal", "Choose normal texture", ".png,.jpg,.jpeg", ".");

            ImGui::SameLine();

            bool useNormal = m_model->GetUseNormal();
            if(ImGui::Checkbox("Use normal", &useNormal))
            {
                m_model->UseNormal(useNormal);
            }

            /* opacity */
            ImGui::Image((void*)(intptr_t)m_model->GetMaterial()->opacity.id, {128, 128});

            ImGui::SameLine();

            if(ImGui::Button("Load opacity"))
                ImGuiFileDialog::Instance()
                    ->OpenDialog("ChooseOpacity", "Choose opacity texture", ".png,.jpg,.jpeg", ".");

            ImGui::SameLine();

            bool useOpacity = m_model->GetUseOpacity();
            if(ImGui::Checkbox("Use opacity", &useOpacity))
            {
                m_model->UseOpacity(useOpacity);
            }
        }
    }

    if (ImGuiFileDialog::Instance()->Display("ChooseModel")) 
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string modelPath = ImGuiFileDialog::Instance()->GetFilePathName();
            LoadModel(modelPath);
        }
    
        ImGuiFileDialog::Instance()->Close();
    }

    if (ImGuiFileDialog::Instance()->Display("ChooseDiffuse")) 
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string path = ImGuiFileDialog::Instance()->GetFilePathName();
            LoadTexture(path, TextureType::Diffuse);
        }
    
        ImGuiFileDialog::Instance()->Close();
    }

    if (ImGuiFileDialog::Instance()->Display("ChooseSpecular")) 
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string path = ImGuiFileDialog::Instance()->GetFilePathName();
            LoadTexture(path, TextureType::Specular);
        }
    
        ImGuiFileDialog::Instance()->Close();
    }

    if (ImGuiFileDialog::Instance()->Display("ChooseNormal")) 
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string path = ImGuiFileDialog::Instance()->GetFilePathName();
            LoadTexture(path, TextureType::Normal);
        }
    
        ImGuiFileDialog::Instance()->Close();
    }

    if (ImGuiFileDialog::Instance()->Display("ChooseOpacity")) 
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string path = ImGuiFileDialog::Instance()->GetFilePathName();
            LoadTexture(path, TextureType::Opacity);
        }
    
        ImGuiFileDialog::Instance()->Close();
    }

    if(ImGui::CollapsingHeader("Actions"))
    {
        if (ImGui::Button("Reset camera position"))
        {            
            m_camera->SetPerspectiveMatrix(m_startFov, m_startWidth,
                m_startHeight, m_startNear, m_startFar);
            m_camera->SetPosition(m_startCameraPosition);
            m_camera->RotateToDirection(GetModelPosition() - m_camera->GetPosition());
        }

        ImGui::SameLine();

        if (ImGui::Button("Reset camera distance"))
        {
            if(m_model)
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
        ImGui::DragFloat("Shiness", &m_light->shiness, 0.5f, 1.f);

        float* color = &m_light->color[0];
        ImGui::ColorPicker4("Light color", color);
        m_light->color = glm::vec4(color[0], color[1], color[2], color[3]);
    }

    if(ImGui::CollapsingHeader("Camera"))
    {
        ImGui::DragFloat("Mouse sensivitity", &m_sensivitity, 0.1f, 0);
        ImGui::DragFloat("Scroll sensivitity", &m_scrollSensivitity, 0.1f, 0);
        
        if(ImGui::DragFloat3("Origin", &m_originPosition[0]))
        {
            m_camera->RotateToDirection(GetModelPosition() - m_camera->GetPosition());
        }
    }

    if(ImGui::CollapsingHeader("Skybox"))
    {
        bool useSkybox = m_renderer->GetUseSkybox();
        if(ImGui::Checkbox("Use Skybox", &useSkybox))
        {
            m_renderer->SetUseSkybox(useSkybox);
        }

        static std::vector<std::string> skyboxPaths;

        //Add 6 text boxes with paths
        //Allow to select paths from OpenFileDialog
        //Do not allow to load texture if all textures are not loaded

        if(skyboxPaths.size() == 5 && ImGui::Button("Load skybox"))
        {
            LoadSkybox(skyboxPaths);
        }
    }

    if(ImGui::CollapsingHeader("Menu settings"))
    {
        ImGui::SliderFloat("Menu Opacity", &m_menuAlpha, 0.f, 1.f);
    }

    ImGui::End();

    EndImgueFrame();
}

glm::vec3 Application::GetModelPosition() 
{
    return (m_model ? m_model->GetPosition() : glm::vec3(0, 0, 0)) + m_originPosition;
}

void Application::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    Application* app = GetInstancePointer(window);

    if(!app->m_model)
        return;

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

    auto cameraUp = app->m_camera->GetUpVector();
    auto cameraRight = app->m_camera->GetRightVector();
    auto modelPos = app->GetModelPosition();

    app->m_camera->RotateAround(modelPos, cameraUp, xoffset * -app->m_sensivitity);
    app->m_camera->RotateAround(modelPos, cameraRight, yoffset * -app->m_sensivitity);

    app->m_camera->RotateToDirection(modelPos - app->m_camera->GetPosition());
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

    if(!app->m_model)
        return;

    if (app->m_imguiIo->WantCaptureMouse)
    {
        return;
    }

    auto direction = app->m_camera->GetForwardVector();
    app->m_camera->Move(direction * app->m_scrollSensivitity * (float)glm::sign(yoffset));
}

void Application::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    Application* app = GetInstancePointer(window);

    app->m_camera->SetPerspectiveMatrix(app->m_startFov,
        width, height, app->m_startNear, app->m_startFar);

    app->m_renderer->SetViewport(0, 0, width, height);
}

Application* Application::GetInstancePointer(GLFWwindow* window)
{
    return static_cast<Application*>(glfwGetWindowUserPointer(window));
}
