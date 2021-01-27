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

    m_isInitialized = true;
}

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
};

struct Mesh
{
    std::vector<Vertex>* vertices;
    std::vector<unsigned int>* indices;

    GLuint VAO;
    GLuint indicesCount;

    Mesh()
    {
        VAO = 0;
        indicesCount = 0;
        vertices = new std::vector<Vertex>();
        indices = new std::vector<unsigned int>();
    }
};

void Application::Start()
{
    CheckInitialization();

    Shader shader = Shader(g_config.rootFolder + "/assets/shaders/basic_vertex.vert",
        g_config.rootFolder + "/assets/shaders/basic_fragment.frag");
    shader.LoadAndCompile();
    
    std::vector<Mesh>* meshes = new std::vector<Mesh>();

    Assimp::Importer aiImporter;
    auto aiScene = aiImporter.ReadFile(g_config.GetModelPath(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_PreTransformVertices);
    
    for (size_t i = 0; i < aiScene->mNumMeshes; i++)
    {
        auto mesh = Mesh();
        auto aiMesh = aiScene->mMeshes[i];

        for (size_t j = 0; j < aiMesh->mNumVertices; j++)
        {
            auto aiPosition = aiMesh->mVertices[j];
            auto aiNormal = aiMesh->mNormals[j];

            mesh.vertices->push_back({ 
                glm::vec3(aiPosition.x, aiPosition.y, aiPosition.z),
                glm::vec3(aiNormal.x, aiNormal.y, aiNormal.z)
            });
        }

        for (size_t j = 0; j < aiMesh->mNumFaces; j++)
        {
            auto aiFace = aiMesh->mFaces[j];

            for (size_t k = 0; k < aiFace.mNumIndices; k++)
            {
                auto index = aiFace.mIndices[k];
                mesh.indices->push_back(index);
            }
        }

        GLuint VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, mesh.vertices->size() * sizeof(Vertex), &mesh.vertices->front(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices->size() * sizeof(unsigned int), &mesh.indices->front(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        mesh.VAO = VAO;
        mesh.indicesCount = mesh.indices->size();

        meshes->push_back(mesh);
    }

    auto modelPosition = glm::vec3(0.f, 0.f, 0.f);
    auto cameraPosition = glm::vec3(50.f, 0.f, 50.f);
    float scale = 0.1f;

    auto modelMatrix = glm::scale(glm::translate(glm::mat4(1.f), modelPosition), glm::vec3(scale, scale, scale));

    auto viewMatrix = glm::lookAt(
        cameraPosition,
        modelPosition,
        glm::vec3(0.f, 1.f, 0.f)
    );

    auto projectionMatrix = glm::perspectiveFov(glm::radians(90.f),
        (float)g_config.viewportSettings.width, (float)g_config.viewportSettings.height,
        0.1f, 100.f);

    auto lightDir = glm::vec3(0.5f, 0.5f, 0.f);

    glClearColor(0.3f, 0.3f, 0.3f, 1.f);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(m_window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();

        shader.UseProgram();

        shader.SetMat4Uniform("ModelMatrix", modelMatrix);
        shader.SetMat4Uniform("ViewMatrix", viewMatrix);
        shader.SetMat4Uniform("ProjectionMatrix", projectionMatrix);
        shader.SetVec3Uniform("LightDir", lightDir);

        for each (Mesh mesh in *meshes)
        {
            glBindVertexArray(mesh.VAO);
            glDrawElements(GL_TRIANGLES, mesh.indicesCount, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

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
