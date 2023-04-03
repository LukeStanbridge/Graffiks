#pragma once
#include "Application.h"
#include "Scene.h"

class Instance
{
public:
	Instance(glm::mat4 transform, Mesh* mesh, aie::ShaderProgram* shader);
	Instance(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale, Mesh* mesh, aie::ShaderProgram* shader);
	void draw(Scene* scene);
	glm::mat4 makeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale);


protected:
	glm::mat4 m_transform;
	Mesh* m_mesh;
	aie::ShaderProgram* m_shader;
};

