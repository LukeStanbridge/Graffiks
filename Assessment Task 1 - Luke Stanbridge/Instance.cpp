#include "Instance.h"

Instance::Instance(glm::mat4 transform, Mesh* mesh, aie::ShaderProgram* shader)
{
	m_transform = transform;
	m_mesh = mesh;
	m_shader = shader;
}

Instance::Instance(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale, Mesh* mesh, aie::ShaderProgram* shader)
{
	m_mesh = mesh;
	m_shader = shader;

	m_transform = makeTransform(position, eulerAngles, scale);
}

void Instance::draw(Scene* scene)
{
	// set the shader pipeline
	m_shader->bind();

	// bind transform and other uniforms
	auto pvm = scene->getCamera()->getProjectionMatrix(scene->getWindowSize().x, scene->getWindowSize().y) * scene->getCamera()->getViewMatrix() * m_transform;
	m_shader->bindUniform("ProjectionViewModel", pvm);
	m_shader->bindUniform("ModelMatrix", m_transform);
	m_shader->bindUniform("AmbientColour", scene->getAmbientLight());
	m_shader->bindUniform("LightColour", scene->getLight().colour);
	m_shader->bindUniform("LightDirection", scene->getLight().direction);
	m_shader->bindUniform("cameraPosition", scene->getCamera()->getPosition());

	int numLights = scene->getNumLights();
	m_shader->bindUniform("numLights", numLights);
	m_shader->bindUniform("PointLightPosition", numLights, scene->getPointLightPositions());
	m_shader->bindUniform("PointLightColour", numLights, scene->getPointLightColours());

	m_mesh->applyMaterial(m_shader);

	// draw mesh
	m_mesh->draw();
}

glm::mat4 Instance::makeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale)
{
	return glm::translate(glm::mat4(1), position)
		* glm::rotate(glm::mat4(1), glm::radians(eulerAngles.z), glm::vec3(0, 0, 1))
		* glm::rotate(glm::mat4(1), glm::radians(eulerAngles.y), glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1), glm::radians(eulerAngles.x), glm::vec3(1, 0, 0))
		* glm::scale(glm::mat4(1), scale);
}
