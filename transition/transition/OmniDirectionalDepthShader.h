#pragma once
#include "ShaderResource.h"
#include <glm/glm.hpp>

class LightNode;

class OmniDirectionalDepthShader :
	public ShaderResource
{
	glm::mat4 view_projection_;
	GLint model_uniform_;
	GLint light_pos_uniform_;
	GLint far_plane_uniform_;
	GLint shadow_transform_uniform_[6];
public:
	OmniDirectionalDepthShader();
	~OmniDirectionalDepthShader();

	void init() override;

	void set_camera_uniforms(const RenderingNode* node) override;
	void set_model_uniforms(const GeometryNode* node) override;
};

