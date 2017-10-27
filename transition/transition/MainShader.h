#pragma once
#include "ShaderResource.h"
#include "ILightShader.h"
#include "TextureResource.h"
#include "GeometryNode.h"
#include "RenderingNode.h"

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
	
	void set_light_ambient(int light_index, const glm::vec3& color) const override;
	void set_light_diffuse(int light_index, const glm::vec3& color) const override;
	void set_light_specular(int light_index, const glm::vec3& color) const override;
	void set_light_linear(const int light_index, const float linear) const override;
	void set_light_quadratic(const int light_index, const float quadratic) const override;
	
	void set_camera_uniforms(const RenderingNode* node) override;
	void set_model_uniforms(const GeometryNode* node) override;
};

