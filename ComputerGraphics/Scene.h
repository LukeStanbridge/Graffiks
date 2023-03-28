#pragma once
#include "Application.h"
#include <list>

class Instance;

class Scene
{
public:
	Scene(Camera* camera, glm::vec2 windowSize, Light& light, glm::vec3 ambientLight);
	~Scene();
	void AddInstance(Instance* instance);
	Camera* getCamera() { return m_camera; }
	glm::vec2 getWindowSize() { return m_windowSize; }
	Light getLight() { return m_light; }
	glm::vec3 getAmbientLight() { return m_ambientLight; }
	void draw();

protected:
	Camera* m_camera;
	glm::vec2 m_windowSize;
	Light m_light;
	glm::vec3 m_ambientLight;
	std::list<Instance*> m_instances;
};

