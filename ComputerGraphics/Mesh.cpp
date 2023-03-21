#include "Mesh.h"
#include "Shader.h"
#include <assimp/scene.h> 
#include <assimp/cimport.h> 
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

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

	// enable third element as texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

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
	vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };
	vertices[3].position = { -0.5f, 0, -0.5f, 1 };
	vertices[4].position = { 0.5f, 0, 0.5f, 1 };
	vertices[5].position = { 0.5f, 0, -0.5f, 1 };

	vertices[0].normal = { 0, 1, 0, 0 };
	vertices[1].normal = { 0, 1, 0, 0 };
	vertices[2].normal = { 0, 1, 0, 0 };
	vertices[3].normal = { 0, 1, 0, 0 };
	vertices[4].normal = { 0, 1, 0, 0 };
	vertices[5].normal = { 0, 1, 0, 0 };

	vertices[0].texCoord = { 0.0f, 1.0f };
	vertices[1].texCoord = { 1.0f, 1.0f };
	vertices[2].texCoord = { 0.0f, 0.0f };
	vertices[3].texCoord = { 0.0f, 0.0f };
	vertices[4].texCoord = { 1.0f, 1.0f };
	vertices[5].texCoord = { 1.0f, 0.0f };

	// fill vertex buffer 
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// enable first element as position 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// enable second element as normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

	// enable third element as texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

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
		// UVs - assume that the first set of UVs are to be used for texture wrapping
		if (mesh->mTextureCoords[0])
		vertices[i].texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, 1 - mesh->mTextureCoords[0][i].y);
		else
		vertices[i].texCoord = glm::vec2(0);
	}
	initialise(numV, vertices, indices.size(), indices.data());

	delete[] vertices;
}

void Mesh::applyMaterial(aie::ShaderProgram* shader)
{
	shader->bindUniform("Ka", Ka);
	shader->bindUniform("Kd", Kd);
	shader->bindUniform("Ks", Ks);
	shader->bindUniform("specularPower", specularPower);

	mapKd.bind(0);
	shader->bindUniform("diffuseTex", 0);
}

void Mesh::loadMaterial(const char* fileName)
{
	std::fstream file(fileName, std::ios::in);
	std::string line;
	std::string header;
	char buffer[256];

	// get the path part of the fileName for use with
	// relative paths for maps later
	std::string directory(fileName);
	int index = directory.rfind('/');
	if (index != -1) directory = directory.substr(0, index + 1);

	while (!file.eof())
	{
		file.getline(buffer, 256);
		line = buffer;
		std::stringstream ss(line, std::stringstream::in | std::stringstream::out);
		if (line.find("Ka") == 0)
			ss >> header >> Ka.x >> Ka.y >> Ka.z;
		else if (line.find("Ks") == 0)
			ss >> header >> Ks.x >> Ks.y >> Ks.z;
		else if (line.find("Kd") == 0)
			ss >> header >> Kd.x >> Kd.y >> Kd.z;
		else if (line.find("Ns") == 0)
			ss >> header >> specularPower;
		else if (line.find("map_Kd") == 0)
		{
			std::string mapFileName;
			ss >> header >> mapFileName;
			mapKd.load((directory + mapFileName).c_str());
		}
	}
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
