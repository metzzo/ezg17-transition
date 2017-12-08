#include "MeshResource.h"
#include <cstring>
#include "glheaders.h"
#include "TextureRenderable.h"

MeshResource *MeshResource::create_sprite(TextureRenderable* resource)
{
	float vertices[] = {
		-1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f
	};

	float normals[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
	};

	float uvs[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 2, // first triangle
		3, 2, 1  // second triangle
	};

	float *quad_vertices = new float[12];
	memcpy(quad_vertices, &vertices, 12 * sizeof(float));

	float *quad_normals = new float[12];
	memcpy(quad_normals, &normals, 12 * sizeof(float));

	float *quad_uvs = new float[8];
	memcpy(quad_uvs, &uvs, 8 * sizeof(float));

	unsigned int *quad_indices = new unsigned int[6];
	memcpy(quad_indices, &indices, 6 * sizeof(unsigned int));

	Material mat;
	mat.set_ambient_color(glm::vec3(1, 1, 1));
	mat.set_diffuse_color(glm::vec3(0, 0, 0));
	mat.set_specular_color(glm::vec3(0, 0, 0));

	mat.set_texture(resource);

	return new MeshResource(
		quad_vertices,
		quad_normals,
		quad_uvs,
		4,
		quad_indices,
		6,
		mat
	);
}

MeshResource::MeshResource(float *vertices, float *normals, float *uvs, const int num_vertices, unsigned int *indices, const int num_indices, const Material& material) {
	this->vao_ = -1;
	this->vbo_positions_ = -1;
	this->vbo_normals_ = -1;
	this->vbo_uvs_ = -1;
	this->ebo_ = -1;

	this->vertices_ = vertices;
	this->normals_ = normals;
	this->uvs_ = uvs;
	this->num_vertices_ = num_vertices;
	this->indices_ = indices;
	this->num_indices_ = num_indices;
	this->material_ = material;
}

MeshResource::~MeshResource()
{
	if (this->vao_ != -1) {
		glDeleteVertexArrays(1, &this->vao_);
		glDeleteBuffers(1, &this->vbo_positions_);
		glDeleteBuffers(1, &this->vbo_normals_);
		glDeleteBuffers(1, &this->ebo_);
	}

	delete this->vertices_;
	delete this->normals_;
	delete this->uvs_;
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
	glGenBuffers(1, &this->vbo_uvs_);
	glGenBuffers(1, &this->ebo_);

	glBindVertexArray(vao_);

	//Bind Positions to Shader-Location 0
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_positions_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*this->num_vertices_ * 3, this->vertices_, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	//Bind UVs to Shader-Location 1
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_uvs_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*this->num_vertices_ * 2, this->uvs_, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

	//Bind Normals to Shader-Location 2
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_normals_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*this->num_vertices_*3, this->normals_, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*this->num_indices_, this->indices_, GL_STATIC_DRAW);

	glBindVertexArray(0);
}
