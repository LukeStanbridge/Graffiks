#pragma once
#include "glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 


class Camera
{
private:
    float m_theta = 0;
    float m_phi = 0;
    glm::vec3 m_position = glm::vec3(-10, 2, 0);

public:
    void update(float deltaTime, GLFWwindow* window);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix(float w, float h);
};

