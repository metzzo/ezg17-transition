#pragma once
#include "ShaderResource.h"
#include "TextureResource.h"

/*
Shader for applying a vertikal or horizontal gaussian filter to an image
*/
class BloomGaussShader : public ShaderResource {

private:
	GLint texture_uniform_;
	GLint horizontal_uniform_;

public:
	BloomGaussShader();
	~BloomGaussShader();

	void init() override;

	void set_camera_uniforms(const RenderingNode* node) override;
	void set_model_uniforms(const GeometryNode* node) override;

	void set_gauss_uniforms(TextureRenderable* texture, bool horizontal);
};
