#include "MeshResource.h"
#include <cstring>
#include <glad/glad.h>

MeshResource::MeshResource()
{
	this->vao_ = 0;
	this->vbo_ = 0;
	this->ebo_ = 0;

	float vertices[] = {
		// positions          // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	this->vertices_ = new float[sizeof(vertices)];
	memcpy(this->vertices_, &vertices, sizeof(vertices));
	this->num_vertices_ = sizeof(vertices) / sizeof(float);

	this->indices_ = new unsigned int[sizeof(indices)];
	memcpy(this->indices_, &indices, sizeof(indices));
	this->num_indices_ = sizeof(indices) / sizeof(unsigned int);


}

MeshResource::~MeshResource()
{
	glDeleteVertexArrays(1, &this->vao_);
	glDeleteBuffers(1, &this->vbo_);
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
	glGenBuffers(1, &this->vbo_);
	glGenBuffers(1, &this->ebo_);

	glBindVertexArray(vao_);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*this->num_vertices_, this->vertices_, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*this->num_indices_, this->indices_, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}
