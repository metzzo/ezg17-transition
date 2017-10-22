#include "MeshResource.h"
#include <cstring>
#include <glad/glad.h>

MeshResource::MeshResource()
{
	this->vao_ = 0;
	this->vbo_positions_ = 0;
	this->vbo_normals_ = 0;
	this->vbo_uvs_ = 0;
	this->ebo_ = 0;

	float vertices[] = {
		// positions          // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
	};

	float normals[] = {
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	float uvs[] = {
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	this->vertices_ = new float[16];
	memcpy(this->vertices_, &vertices, sizeof(vertices));
	this->num_vertices_ = 4;

	this->normals_ = new float[16];
	memcpy(this->normals_, &normals, sizeof(normals));

	this->uvs_ = new float[8];
	memcpy(this->uvs_, &uvs, sizeof(uvs));

	this->indices_ = new unsigned int[6];
	memcpy(this->indices_, &indices, sizeof(indices));
	this->num_indices_ = 6;


}

MeshResource::MeshResource(float *vertices, float *normals, float *uvs, int num_vertices, unsigned int *indices, int num_indices) {
	this->vao_ = 0;
	this->vbo_positions_ = 0;
	this->vbo_normals_ = 0;
	this->vbo_uvs_ = 0;
	this->ebo_ = 0;

	this->vertices_ = vertices;
	this->normals_ = normals;
	this->uvs_ = uvs;
	this->num_vertices_ = num_vertices;
	this->indices_ = indices;
	this->num_indices_ = num_indices;
}

MeshResource::~MeshResource()
{
	glDeleteVertexArrays(1, &this->vao_);
	glDeleteBuffers(1, &this->vbo_positions_);
	glDeleteBuffers(1, &this->vbo_normals_);
	glDeleteBuffers(1, &this->ebo_);

	delete this->vertices_;
	delete this->indices_;
}

int MeshResource::get_resource_id() const
{
	return this->vao_;
}

void MeshResource::init()
{
	glGenVertexArrays(1, &this->vao_);
	glGenBuffers(1, &this->vbo_positions_);
	glGenBuffers(1, &this->vbo_normals_);
	glGenBuffers(1, &this->ebo_);

	glBindVertexArray(vao_);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_positions_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*this->num_vertices_*4, this->vertices_, GL_STATIC_DRAW);

	//Bind Positions to Shader-Location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_normals_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*this->num_vertices_*4, this->normals_, GL_STATIC_DRAW);

	//Bind Normals to Shader-Location 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_uvs_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*this->num_vertices_ * 2, this->uvs_, GL_STATIC_DRAW);

	//Bind Normals to Shader-Location 3
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*this->num_indices_, this->indices_, GL_STATIC_DRAW);


}
