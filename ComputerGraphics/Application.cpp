#include "Application.h"
#include "Instance.h"
#include "Scene.h"

const int windowWidth = 1600;
const int windowHeight = 1000;

Application* Application::s_instance = nullptr;

bool Application::startup()
{
    if (glfwInit() == false) return -1;

    m_window = glfwCreateWindow(windowWidth, windowHeight, "Computer Graphics", nullptr, nullptr);

    if (m_window == nullptr)
    {
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGL())
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
        return -3;
    }

    Gizmos::create(10000, 10000, 0, 0);
    s_instance = this;
    glfwSetCursorPosCallback(m_window, &Application::SetMousePosition);
    glm::vec2 mousepos = Application::get()->getMousePosition();

    m_view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
    m_projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

    m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "./Shaders/normalmap.vert");
    m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "./Shaders/normalmap.frag");

    if (m_normalMapShader.link() == false)
    {
        printf("Shader Error: %s\n", m_normalMapShader.getLastError());
        return false;
    }

    m_light.direction = glm::normalize(vec3(-1));
    m_light.colour = { 1, 1, 1 };
    m_ambientLight = { 0.25f, 0.25f, 0.25f };

    m_spearMesh.initialiseFromFile("soulspear/soulspear.obj");
    m_spearMesh.loadMaterial("soulspear/soulspear.mtl");

    glm::mat4 spearTransform = {
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1};

    Light light;
    light.colour = { 1, 1, 1 };
    light.direction = vec3(1, -1, 1);
    m_scene = new Scene(&m_camera, glm::vec2(windowWidth, windowHeight), light, glm::vec3(0.25f, 0.25f, 0.25f));

    //create 10 instances
    for (int i = 0; i < 10; i++)
    {
        m_scene->AddInstance(new Instance(glm::vec3(i), glm::vec3(0), glm::vec3(1), &m_spearMesh, &m_normalMapShader));   
    }

   /* m_scene->AddInstance(new Instance(spearTransform, &m_spearMesh, &m_normalMapShader));*/

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);


    glClearColor(0.25f, 0.25f, 0.25f, 1);
    glEnable(GL_DEPTH_TEST); // enables the depth buffer 
    
	return true;
}

bool Application::update()
{
    m_camera.update(0.1f, m_window);
    m_lastMousePosition = m_mousePosition;

    float time = glfwGetTime();

    return glfwWindowShouldClose(m_window) == false && glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS;
}

void Application::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Gizmos::clear();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    Gizmos::addTransform(glm::mat4(1));
    glm::mat4 pv = m_camera.getProjectionMatrix(windowWidth, windowHeight) * m_camera.getViewMatrix();

    m_scene->draw();
    
    //different colours
    vec4 white(1);
    vec4 black(0, 0, 0, 1);

    //create background griddy
    for (int i = 0; i < 21; ++i)
    {
        Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);

        Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
    }

    //GUI Creation
    ImGui::Begin("Light Settings");
    ImGui::DragFloat3("Sunlight Direction", &m_light.direction[0], 0.1f, -1.0f, 1.0f);
    ImGui::DragFloat3("Sunlight Colour", &m_light.colour[0], 0.1f, 0.0f, 2.0f);
    ImGui::End();

    Gizmos::draw(pv);

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // must be at the end of drawing or it won't work
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Application::shutdown()
{
    Gizmos::destroy();
    delete m_scene;

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Application::SetMousePosition(GLFWwindow* window, double x, double y)
{
    s_instance->m_mousePosition.x = (float)x;
    s_instance->m_mousePosition.y = (float)y;
}
