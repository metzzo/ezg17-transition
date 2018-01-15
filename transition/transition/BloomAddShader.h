#pragma once
#include "ShaderResource.h"
#include "TextureResource.h"

/*
Shader for adding two images
*/
class BloomAddShader : public ShaderResource {

private:
	GLint texture1_uniform_;
	GLint texture2_uniform_;
	GLint addintensity_uniform_;
	GLint end_tex_uniform_;
	GLint end_tex_intensity_uniform_;

public:
	BloomAddShader();
	~BloomAddShader();

	void init() override;

	void set_camera_uniforms(const RenderingNode* node) override;
	void set_model_uniforms(const GeometryNode* node) override;

	void set_textures(TextureRenderable *tex1, TextureRenderable *tex2, float addintensity, TextureRenderable * end_tex, float end_tex_intensity);
};
