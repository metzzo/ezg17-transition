#include "DummyShader.h"

DummyShader::DummyShader() : ShaderResource("assets/shaders/postprocess.vs", "assets/shaders/dummy.fs")
{
	this->texture_uniform_ = -1;
}

DummyShader::~DummyShader()
{
}

void DummyShader::init()
{
	ShaderResource::init();
	this->texture_uniform_ = get_uniform("tex");
}

void DummyShader::set_camera_uniforms(const RenderingNode * node)
{
}

void DummyShader::set_model_uniforms(const GeometryNode * node)
{
}

void DummyShader::set_texture(TextureRenderable * tex)
{
	tex->bind(0);
	glUniform1i(texture_uniform_, 0);
}
