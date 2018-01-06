#include "BloomAddShader.h"

BloomAddShader::BloomAddShader() : ShaderResource("assets/shaders/postprocess.vs", "assets/shaders/bloom_add.fs")
{
	this->texture1_uniform_ = -1;
	this->texture2_uniform_ = -1;
}

BloomAddShader::~BloomAddShader()
{
}

void BloomAddShader::init()
{
	ShaderResource::init();
	this->texture1_uniform_ = get_uniform("tex1");
	this->texture2_uniform_ = get_uniform("tex2");
	this->addintensity_uniform_ = get_uniform("addintensity");
}

void BloomAddShader::set_camera_uniforms(const RenderingNode * node)
{
}

void BloomAddShader::set_model_uniforms(const GeometryNode * node)
{
}

void BloomAddShader::set_textures(TextureRenderable * tex1, TextureRenderable * tex2, float addintensity)
{
	tex1->bind(0);
	glUniform1i(texture1_uniform_, 0);
	tex2->bind(1);
	glUniform1i(texture2_uniform_, 1);
	glUniform1f(addintensity_uniform_, addintensity);
}
