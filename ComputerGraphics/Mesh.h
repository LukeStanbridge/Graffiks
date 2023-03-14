#pragma once
#include "glad.h"
#include "Gizmos.h"
#include <glm/fwd.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/ext.hpp>

class Mesh
{
public:

	Mesh() : triCount(0), vao(0), vbo(0), ibo(0) {}
	virtual ~Mesh();

	struct Vertex 
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 texCoord;
	};

	void initialiseQuad();

	virtual void draw();

protected:

	unsigned int triCount;
	unsigned int vao, vbo, ibo;
	
};

