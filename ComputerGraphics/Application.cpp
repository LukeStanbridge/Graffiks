#include "Application.h"

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
    m_sun = mat4(1);
    m_planet1 = mat4(1);
    m_planet2 = mat4(1);
    m_planet3 = mat4(1);

    m_shader.loadShader(aie::eShaderStage::VERTEX, "./Shaders/simple.vert");
    m_shader.loadShader(aie::eShaderStage::FRAGMENT, "./Shaders/simple.frag");
    m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./Shaders/phong.vert");
    m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./Shaders/phong.frag");
    if (m_shader.link() == false)
    {
        printf("Shader Error: %s\n", m_shader.getLastError());
        return false;
    }
    if (m_phongShader.link() == false)
    {
        printf("Shader Error: %s\n", m_phongShader.getLastError());
        return false;
    }

    m_light.direction = glm::normalize(vec3(-1));
    m_light.colour = { 1, 1, 1 };
    m_ambientLight = { 0.25f, 0.25f, 0.25f };
    m_gridTexture.load("textures/earth_cloud.jpg");

    m_quadMesh.initialiseQuad();
    m_quadTransform = {
    10,0,0,0,
    0,10,0,0,
    0,0,10,0,
    0,0,0,1 };

    m_bunnyMesh.initialiseFromFile("soulspear/soulspear.obj");
    m_bunnyMesh.loadMaterial("soulspear/soulspear.mtl");
    m_bunnyTransform = {
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1 };

    glClearColor(0.25f, 0.25f, 0.25f, 1);
    glEnable(GL_DEPTH_TEST); // enables the depth buffer 

	return true;
}

bool Application::update()
{
    m_camera.update(0.1f, m_window);
    m_lastMousePosition = m_mousePosition;

    // query time since application started
    float time = glfwGetTime();
    // rotate light
    m_light.direction = glm::normalize(vec3(glm::cos(time * 2), glm::sin(time * 2), 0));

    return glfwWindowShouldClose(m_window) == false && glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS;
}

void Application::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Gizmos::clear();

    Gizmos::addTransform(glm::mat4(1));
    
    //different colours
    vec4 white(1);
    vec4 black(0, 0, 0, 1);
    vec4 red(1, 0, 0, 1);
    vec4 coral(1.0f, 0.5f, 0.31f, 1);
    vec4 blue(0, 0, 1, 1);
    vec4 green(0, 1, 0, 1);
    vec4 yellow(1, 1, 0, 1);
    vec4 magenta(1, 0, 1, 1);
    vec4 cyan(0, 1, 1, 1);

    glm::mat4 pv = m_camera.getProjectionMatrix(windowWidth, windowHeight) * m_camera.getViewMatrix();

    m_phongShader.bind();

    // bind light
    m_phongShader.bindUniform("AmbientColour", m_ambientLight);
    m_phongShader.bindUniform("LightColour", m_light.colour);
    m_phongShader.bindUniform("LightDirection", m_light.direction);
    m_phongShader.bindUniform("cameraPosition", vec3(glm::inverse(m_viewMatrix)[3]));
    // or m_phongShader.bindUniform("cameraPosition", m_camera.getPosition());

    // bind transform 
    bindTransform(pv, m_phongShader, m_bunnyTransform);

    /*m_bunnyMesh.Ka = vec3(0.8f, 0.2, 0);
    m_bunnyMesh.Kd - vec3(0.8f, 0.8f, 0);
    m_bunnyMesh.Ks - vec3(1, 1, 1);
    m_bunnyMesh.specularPower = 32;*/

    /*m_phongShader.bindUniform("Ka", vec3(0.25f, 0.25f, 0));
    m_phongShader.bindUniform("Ks", vec3(1.0f));
    m_phongShader.bindUniform("Kd", vec3(0.5f, 0.5f, 0));
    m_phongShader.bindUniform("specularPower", 32.0f);*/

    m_gridTexture.bind(0);
    m_phongShader.bindUniform("diffuseTex", 0);

    // draw quad 
    m_bunnyMesh.applyMaterial(&m_phongShader);
    m_bunnyMesh.draw();
    bindTransform(pv, m_phongShader, m_quadTransform);
    m_quadMesh.draw();

    //create background griddy
    for (int i = 0; i < 21; ++i)
    {
        Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);

        Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
    }

    ////solar system
    //Gizmos::addSphere(vec3(0, 5, 0), 2, 20, 20, red, &m_sun);
    //m_sun = glm::rotate(m_sun, glm::radians(0.1f), vec3(0, 1, 0));

    //Gizmos::addSphere(vec3(-5, 5, -5), 0.8f, 20, 50, coral, &m_planet1);
    //m_planet1 = glm::rotate(m_planet1, glm::radians(2.0f), vec3(0.5, 0, 0.5));

    //Gizmos::addSphere(vec3(5, 5, 5), 0.8f, 20, 20, blue, &m_planet2);
    //m_planet2 = glm::rotate(m_planet2, glm::radians(0.5f), vec3(0.7, 0.5, 0.5));

    //Gizmos::addSphere(vec3(6, 5, -3), 0.8f, 20, 20, green, &m_planet3);
    //m_planet3 = glm::rotate(m_planet3, glm::radians(1.0f), vec3(0, 0, 0.5));

    //Gizmos::addArcRing(vec3(0, 5, 0), 2, 3, 3.3f, 3.5f, 30, yellow, &m_sun);

    Gizmos::draw(pv);

    // must be at the end of drawing or it won't work
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Application::shutdown()
{
    Gizmos::destroy();
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Application::SetMousePosition(GLFWwindow* window, double x, double y)
{
    s_instance->m_mousePosition.x = (float)x;
    s_instance->m_mousePosition.y = (float)y;
}

void Application::bindTransform(glm::mat4 pv, aie::ShaderProgram& shader, glm::mat4 transform)
{
    auto pvm = pv * transform;
    shader.bindUniform("ProjectionViewModel", pvm);

    // bind transforms for lighting 
    shader.bindUniform("ModelMatrix", transform);
}
