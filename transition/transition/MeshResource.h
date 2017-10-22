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

	int get_num_indices() const
	{
		return num_indices_;
	}

	int get_num_vertices() const
	{
		return num_vertices_;
	}
};

