#include "BloomGaussShader.h"
#include "GeometryNode.h"

BloomGaussShader::BloomGaussShader() : ShaderResource("assets/shaders/postprocess.vs", "assets/shaders/bloom_gauss.fs")
{
	this->texture_uniform_ = -1;
	this->horizontal_uniform_ = -1;
}

BloomGaussShader::~BloomGaussShader()
{
}

void BloomGaussShader::init()
{
	ShaderResource::init();
	this->texture_uniform_ = get_uniform("image");
	this->horizontal_uniform_ = get_uniform("horizontal");
}

void BloomGaussShader::set_camera_uniforms(const RenderingNode * node)
{
	//Nothing
}

void BloomGaussShader::set_model_uniforms(const GeometryNode * node)
{
	//Nothing
}



void BloomGaussShader::set_gauss_uniforms(TextureRenderable* texture, bool horizontal)
{
	texture->bind(0);
	glUniform1i(texture_uniform_, 0);
	glUniform1i(horizontal_uniform_, horizontal);
}
