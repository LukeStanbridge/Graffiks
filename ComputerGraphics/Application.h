#pragma once
#include "Shader.h"
#include "Mesh.h"
#include "glad.h"
#include "Gizmos.h"
#include <glm/fwd.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

class Application
{
public:
	bool startup();
	bool update();
	void draw();
	void shutdown();

protected:
	glm::mat4 m_view;
	glm::mat4 m_projection;

	GLFWwindow* m_window;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	aie::ShaderProgram m_shader;
	Mesh m_quadMesh;
	glm::mat4 m_quadTransform;
	
	glm::mat4 m_sun;
	glm::mat4 m_planet1;
	glm::mat4 m_planet2;
	glm::mat4 m_planet3;
};

