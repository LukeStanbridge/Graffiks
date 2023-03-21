#pragma once
#include "Shader.h"
#include "Mesh.h"
#include "glad.h"
#include "Gizmos.h"
#include "Camera.h"
#include "Texture.h"
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

	// singleton pattern
	static Application* get() { return s_instance; }
	glm::vec2 getMousePosition() { return m_mousePosition; }
	glm::vec2 getMouseDelta() { return m_mousePosition - m_lastMousePosition; }

	// set up mouse input
	static void SetMousePosition(GLFWwindow* window, double x, double y);

	void bindTransform(glm::mat4, aie::ShaderProgram&, glm::mat4);


protected:
	static Application* s_instance;
	glm::mat4 m_view;
	glm::mat4 m_projection;

	GLFWwindow* m_window;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	aie::ShaderProgram m_shader;
	aie::ShaderProgram m_phongShader;
	aie::Texture m_gridTexture;

	Mesh m_quadMesh;
	glm::mat4 m_quadTransform;
	Mesh m_bunnyMesh;
	glm::mat4 m_bunnyTransform;
	
	glm::mat4 m_sun;
	glm::mat4 m_planet1;
	glm::mat4 m_planet2;
	glm::mat4 m_planet3;

	glm::vec2 m_mousePosition;
	glm::vec2 m_lastMousePosition;
	Camera m_camera;

	struct Light 
	{ 
		glm::vec3 direction;
		glm::vec3 colour;
	};

	Light m_light;
	glm::vec3 m_ambientLight;
};

