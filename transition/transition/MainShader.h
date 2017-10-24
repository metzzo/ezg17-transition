#pragma once
#include "ShaderResource.h"
#include "TextureResource.h"

class MainShader :
	public ShaderResource
{
	GLint model_uniform_;
	GLint view_uniform_;
	GLint projection_uniform_;
	GLint diffuse_texture_uniform_;
public:
	MainShader();
	~MainShader();

	void init() override;

	void set_model(const glm::mat4& model) const;
	void set_view(const glm::mat4& view) const;
	void set_projection(const glm::mat4& projection) const;
	void set_diffuse_texture(TextureResource* texture) const;
};

