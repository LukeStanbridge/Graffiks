#include "Application.h"

const int windowWidth = 1600;
const int windowHeight = 1000;

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
    m_view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
    m_projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
    m_sun = mat4(1);
    m_planet1 = mat4(1);
    m_planet2 = mat4(1);
    m_planet3 = mat4(1);

    m_shader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");
    m_shader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");
    if (m_shader.link() == false)
    {
        printf("Shader Error: %s\n", m_shader.getLastError());
        return false;
    }

    //m_quadMesh.initialiseQuad();

    // make the quad 10 units wide 
    m_quadTransform = {
     10,0,0,0,
     0,10,0,0,
     0,0,10,0,
     0,0,0,1 }; 

    //// define 6 vertices for 2 triangles 
    //Mesh::Vertex vertices[6];
    //vertices[0].position = { -0.5f, 0, 0.5f, 1 };
    //vertices[1].position = { 0.5f, 0, 0.5f, 1 };
    //vertices[2].position = { -0.5f, 0, -0.5f, 1 };

    //vertices[3].position = { -0.5f, 0, -0.5f, 1 };
    //vertices[4].position = { 0.5f, 0, 0.5f, 1 };
    //vertices[5].position = { 0.5f, 0, -0.5f, 1 };

    //m_quadMesh.initialise(6, vertices);

   /* m_quadTransform = {
     10,0,0,0,
     0,10,0,0,
     0,0,10,0,
     0,0,0,1 }; */

    // define 4 vertices for 2 triangles 
    Mesh::Vertex vertices[4];
    vertices[0].position = { -0.5f, 0, 0.5f, 1 };
    vertices[1].position = { 0.5f, 0, 0.5f, 1 };
    vertices[2].position = { -0.5f, 0, -0.5f, 1 };
    vertices[3].position = { 0.5f, 0, -0.5f, 1 };

    unsigned int indices[6] = { 0, 1, 2, 2, 1, 3 };

    m_quadMesh.initialise(4, vertices, 6, indices);

    glClearColor(0.25f, 0.25f, 0.25f, 1);
    glEnable(GL_DEPTH_TEST); // enables the depth buffer 

	return true;
}

bool Application::update()
{
    //exits game with esc key press
    if ((glfwWindowShouldClose(m_window) == false && glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS))
    {
        return true;
    }
   
    return false;
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

    // bind shader 
    m_shader.bind();

    // bind transform 
    auto pvm = m_projection * m_view * m_quadTransform;
    m_shader.bindUniform("ProjectionViewModel", pvm);

    // draw quad 
    m_quadMesh.draw();

    //create backgorund griddy
    for (int i = 0; i < 21; ++i)
    {
        Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);

        Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
    }

    //solar system
    Gizmos::addSphere(vec3(0, 3, 0), 2, 20, 20, red, &m_sun);
    m_sun = glm::rotate(m_sun, glm::radians(0.1f), vec3(0, 1, 0));

    Gizmos::addSphere(vec3(-5, 3, -5), 0.8f, 20, 50, coral, &m_planet1);
    m_planet1 = glm::rotate(m_planet1, glm::radians(2.0f), vec3(0.5, 0, 0.5));

    Gizmos::addSphere(vec3(5, 3, 5), 0.8f, 20, 20, blue, &m_planet2);
    m_planet2 = glm::rotate(m_planet2, glm::radians(0.5f), vec3(0.7, 0.5, 0.5));

    Gizmos::addSphere(vec3(6, 3, -3), 0.8f, 20, 20, green, &m_planet3);
    m_planet3 = glm::rotate(m_planet3, glm::radians(1.0f), vec3(0, 0, 0.5));

    Gizmos::addArcRing(vec3(0, 3, 0), 2, 3, 3.3f, 3.5f, 30, yellow, &m_sun);

    Gizmos::draw(m_projection * m_view);

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
