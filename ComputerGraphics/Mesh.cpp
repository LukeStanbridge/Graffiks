#include "Mesh.h"
#include <assimp/scene.h> 
#include <assimp/cimport.h> 
#include <vector>

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Mesh::initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount, unsigned int* indices)
{
	assert(vao == 0);

	// generate buffers 
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// bind vertex array aka a mesh wrapper 
	glBindVertexArray(vao);

	// bind vertex buffer 
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// fill vertex buffer 
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// enable first element as position 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// enable second element as normal 
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

	// bind indices if there are any 
	if (indexCount != 0) {
		glGenBuffers(1, &ibo);

		// bind vertex buffer 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		// fill vertex buffer 
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		triCount = indexCount / 3;
	}
	else 
	{
		triCount = vertexCount / 3;
	}

	// unbind buffers 
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::initialiseQuad()
{
	// check that the mesh is not initialized already 
	assert(vao == 0);

	// generate buffers 
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// bind vertex array aka a mesh wrapper 
	glBindVertexArray(vao);

	// bind vertex buffer 
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// define 6 vertices for 2 triangles 
	Vertex vertices[6];
	/*vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };

	vertices[3].position = { -0.5f, 0, -0.5f, 1 };
	vertices[4].position = { 0.5f, 0, 0.5f, 1 };
	vertices[5].position = { 0.5f, 0, -0.5f, 1 };*/

	vertices[0].normal = { 0, 1, 0, 0 };
	vertices[1].normal = { 0, 1, 0, 0 };
	vertices[2].normal = { 0, 1, 0, 0 };
	vertices[3].normal = { 0, 1, 0, 0 };
	vertices[4].normal = { 0, 1, 0, 0 };
	vertices[5].normal = { 0, 1, 0, 0 };

	// fill vertex buffer 
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// enable first element as position 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// unbind buffers 
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// quad has 2 triangles 
	triCount = 2;
}

void Mesh::initialiseFromFile(const char* filename)
{
	// read vertices from the model 
	const aiScene* scene = aiImportFile(filename, 0);

	// just use the first mesh we find for now 
	aiMesh* mesh = scene->mMeshes[0];

	// extract indicies from the first mesh 
	int numFaces = mesh->mNumFaces;
	std::vector<unsigned int> indices;

	for (int i = 0; i < numFaces; i++)
	{
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);

		// generate a second triangle for quads 
		if (mesh->mFaces[i].mNumIndices == 4)
		{
			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back(mesh->mFaces[i].mIndices[3]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
		}
	}

	// extract vertex data 
	int numV = mesh->mNumVertices;
	Vertex* vertices = new Vertex[numV];
	for (int i = 0; i < numV; i++)
	{
		vertices[i].position = glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1);
		vertices[i].normal = glm::vec4(mesh->mNormals[i].x, mesh -> mNormals[i].y, mesh->mNormals[i].z, 0);
		// TODO  UVs 
	}
	initialise(numV, vertices, indices.size(), indices.data());

	delete[] vertices;
}

void Mesh::draw()
{
	glBindVertexArray(vao);
	// using indices or just vertices? 
	if (ibo != 0)
		glDrawElements(GL_TRIANGLES, 3 * triCount, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, 3 * triCount);
}
