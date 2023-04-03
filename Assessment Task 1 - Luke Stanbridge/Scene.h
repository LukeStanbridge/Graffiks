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
	Light getLight() { return m_sunLight; }
	glm::vec3 getAmbientLight() { return m_ambientLight; }

	int getNumLights() { return (int)m_pointLights.size(); }
	glm::vec3* getPointLightPositions() { return &m_pointLightPositions[0]; }
	glm::vec3* getPointLightColours() { return &m_pointLightColours[0]; }
	std::vector<Light>& getPointLights() { return m_pointLights; }
	float* getPointLightDirection(int index) { return &m_pointLights[index].direction[0]; }

	void draw();

protected:
	Camera* m_camera;
	glm::vec2 m_windowSize;
	Light m_sunLight;
	std::vector<Light> m_pointLights;
	glm::vec3 m_ambientLight;
	std::list<Instance*> m_instances;

	static constexpr int MAX_LIGHTS = 4;
	glm::vec3 m_pointLightPositions[MAX_LIGHTS];
	glm::vec3 m_pointLightColours[MAX_LIGHTS];
};

