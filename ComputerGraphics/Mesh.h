#pragma once
#include "glad.h"
#include "Gizmos.h"
#include "Texture.h"
#include <glm/fwd.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/ext.hpp>
#include <vector>

namespace aie { class ShaderProgram; }
using aie::Gizmos;

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
		glm::vec4 tangent;
	};

	void initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount = 0, unsigned int* indices = nullptr);

	void initialiseQuad();

	void initialiseFromFile(const char* filename);

	void applyMaterial(aie::ShaderProgram* shader);

	void loadMaterial(const char* fileName);

	void calculateTangents(Vertex* vertices, unsigned int vertexCount, const std::vector<unsigned int>& indices);

	virtual void draw();

	glm::vec3 Ka; // ambient colour of the surface
	glm::vec3 Kd; // diffuse colour of the surface
	glm::vec3 Ks; // specular colour of the surface
	float specularPower; // tightness of specular highlights

	aie::Texture mapKd; // diffuse texture map
	aie::Texture mapKs; // specular texture map
	aie::Texture mapBump; // normal map

protected:

	unsigned int triCount;
	unsigned int vao, vbo, ibo;
};

