#include "Scene.h"
#include "Instance.h"

Scene::Scene(Camera* camera, glm::vec2 windowSize, Light& light, glm::vec3 ambientLight)
{
	m_camera = camera;
	m_windowSize = windowSize;
	m_light = light;
	m_ambientLight = ambientLight;
}

Scene::~Scene()
{
	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
		delete* it;
}

void Scene::AddInstance(Instance* instance)
{
	m_instances.push_back(instance);
}

void Scene::draw()
{
	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
	{
		Instance* instance = *it;
		instance->draw(this);
	}
}
