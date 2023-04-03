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
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

struct Light
{
	Light() {};

	glm::vec3 direction;
	glm::vec3 colour;

	Light(vec3 pos, vec3 col, float intensity)
	{
		direction = pos;
		colour = col * intensity;
	};
};

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

protected:
	static Application* s_instance;
	glm::mat4 m_view;
	glm::mat4 m_projection;

	GLFWwindow* m_window;

	aie::ShaderProgram m_normalMapShader;

	Mesh m_spearMesh;
	class Instance* m_spearInstance;

	glm::vec2 m_mousePosition;
	glm::vec2 m_lastMousePosition;
	Camera m_camera;

	Light m_light;
	glm::vec3 m_ambientLight;

	class Scene* m_scene;
};

