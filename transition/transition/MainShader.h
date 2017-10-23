#pragma once
#include "ShaderResource.h"
#include "ILightShader.h"

class MainShader :
	public ShaderResource,
	public ILightShader
{
	GLint model_uniform_;
	GLint view_uniform_;
	GLint projection_uniform_;
public:
	MainShader();
	~MainShader();

	void init() override;

	void set_model(const glm::mat4& model) const;
	void set_view(const glm::mat4& view) const;
	void set_projection(const glm::mat4& projection) const;

	void set_light_ambient(int light_index, const glm::vec3& color) const override;
	void set_light_diffuse(int light_index, const glm::vec3& color) const override;
	void set_light_specular(int light_index, const glm::vec3& color) const override;
	void set_light_linear(const int light_index, const float linear) const override;
	void set_light_quadratic(const int light_index, const float quadratic) const override;
};

