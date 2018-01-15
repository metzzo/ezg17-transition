#include "BloomAddShader.h"

BloomAddShader::BloomAddShader() : ShaderResource("assets/shaders/postprocess.vs", "assets/shaders/bloom_add.fs")
{
	this->texture1_uniform_ = -1;
	this->texture2_uniform_ = -1;
	this->end_tex_uniform_ = -1;
}

BloomAddShader::~BloomAddShader()
{
}

void BloomAddShader::init()
{
	ShaderResource::init();
	this->texture1_uniform_ = get_uniform("tex1");
	this->texture2_uniform_ = get_uniform("tex2");
	this->end_tex_uniform_ = get_uniform("end_tex");
	this->addintensity_uniform_ = get_uniform("addintensity");
	this->end_tex_intensity_uniform_ = get_uniform("end_tex_intensity");
}

void BloomAddShader::set_camera_uniforms(const RenderingNode * node)
{
}

void BloomAddShader::set_model_uniforms(const GeometryNode * node)
{
}

void BloomAddShader::set_textures(TextureRenderable * tex1, TextureRenderable * tex2, float addintensity, TextureRenderable * end_tex, float end_tex_intensity)
{
	tex1->bind(0);
	glUniform1i(texture1_uniform_, 0);
	tex2->bind(1);
	glUniform1i(texture2_uniform_, 1);
	glUniform1f(addintensity_uniform_, addintensity);
	end_tex->bind(2);
	glUniform1i(end_tex_uniform_, 2);
	glUniform1f(end_tex_intensity_uniform_, end_tex_intensity);
}
