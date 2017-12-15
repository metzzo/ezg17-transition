#pragma once
#include "ShaderResource.h"
#include <glm/glm.hpp>

class LightNode;

class DepthOnlyShader :
	public ShaderResource
{
	GLint mvp_uniform_;
	glm::mat4 view_projection_;
public:
	DepthOnlyShader();
	~DepthOnlyShader();

	void init() override;

	void set_camera_uniforms(const RenderingNode* node) override;
	void set_model_uniforms(const GeometryNode* node) override;
};

