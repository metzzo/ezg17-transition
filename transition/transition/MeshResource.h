#pragma once
#include "IResource.h"
#include <glad/glad.h>

class MeshResource : public IResource
{
	float *vertices_;
	int num_vertices_;

	unsigned int *indices_;
	int num_indices_;
	GLuint vao_;
	GLuint vbo_;
	GLuint ebo_;
public:
	MeshResource();
	~MeshResource();

	int get_resource_id() const override;
	void init() override;
};

