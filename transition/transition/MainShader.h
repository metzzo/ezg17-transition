#pragma once
#include "ShaderResource.h"
#include "ILightShader.h"
#include "GeometryNode.h"
#include "RenderingNode.h"

class LightNode;

class MainShader :
	public ShaderResource,
	public ILightShader
{
	GLint model_uniform_;
	GLint view_uniform_;
	GLint projection_uniform_;
	GLint diffuse_texture_uniform_;
public:
	MainShader();
	~MainShader();

	void init() override;
	
	
	void set_camera_uniforms(const RenderingNode* node) override;
	void set_model_uniforms(const GeometryNode* node) override;
	void set_light_uniforms(const std::vector<LightNode*>& light_nodes) override;
};

