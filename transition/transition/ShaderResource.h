#pragma once
#include "IResource.h"
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
class RenderingNode;
class GeometryNode;

class ShaderResource :
	public IResource
{
	const char* vertex_path_;
	const char* fragment_path_;
	const char* geometry_path_;

	GLuint program_id_;

	static void check_compile_errors(GLuint shader, std::string type);
public:
	ShaderResource(const char *vertex_path, const char *fragment_path, const char *geometry_path = nullptr);
	~ShaderResource();

	int get_resource_id() const override;
	void init() override;
	virtual void set_camera_uniforms(const RenderingNode* node) = 0;
	virtual void set_model_uniforms(const GeometryNode* node) = 0;
};

