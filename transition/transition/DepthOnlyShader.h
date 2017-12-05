#pragma once
#include "ShaderResource.h"
#include <vector>

class LightNode;

class DepthOnlyShader :
	public ShaderResource
{
	GLint model_uniform_;
	GLint view_uniform_;
	GLint projection_uniform_;
public:
	DepthOnlyShader();
	~DepthOnlyShader();

	void init() override;

	void set_camera_uniforms(const RenderingNode* node) override;
	void set_model_uniforms(const GeometryNode* node) override;
};

