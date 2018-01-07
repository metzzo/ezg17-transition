#pragma once
#include "ShaderResource.h"
#include <glm/glm.hpp>

class LightNode;

class DirectionalDepthShader :
	public ShaderResource
{
	GLint mvp_uniform_;
	GLint has_alpha_tex_uniform_;
	GLint alpha_tex_uniform_;
	glm::mat4 view_projection_;
public:
	DirectionalDepthShader();
	~DirectionalDepthShader();

	void init() override;

	void set_camera_uniforms(const RenderingNode* node) override;
	void set_model_uniforms(const GeometryNode* node) override;
};

