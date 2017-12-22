#include "DemoParticleShader.h"
#include "RenderingNode.h"
#include "GeometryNode.h"

DemoParticleShader::DemoParticleShader() : ShaderResource("assets/shaders/demo_part.vs", "assets/shaders/demo_part.fs", "assets/shaders/demo_part.gs")
											//ShaderResource("assets/shaders/test_simple.vs", "assets/shaders/test_simple.fs")
{
	model_uniform_ = -1;
	view_uniform_ = -1;
	projection_uniform_ = -1;
}

DemoParticleShader::~DemoParticleShader()
{
}

void DemoParticleShader::init()
{
	ShaderResource::init();
	this->model_uniform_ = get_uniform("mvp.model");
	this->view_uniform_ = get_uniform("mvp.view");
	this->projection_uniform_ = get_uniform("mvp.projection");
}

void DemoParticleShader::set_camera_uniforms(const RenderingNode * node)
{
	glUniformMatrix4fv(this->view_uniform_, 1, GL_FALSE, &node->get_view_matrix()[0][0]);
	glUniformMatrix4fv(this->projection_uniform_, 1, GL_FALSE, &node->get_projection_matrix()[0][0]);
}

void DemoParticleShader::set_model_uniforms(const GeometryNode * node)
{
	glUniformMatrix4fv(this->model_uniform_, 1, GL_FALSE, &node->get_transformation()[0][0]);
}

void DemoParticleShader::set_modelmat_uniforms(const glm::mat4 & trafo)
{
	glUniformMatrix4fv(this->model_uniform_, 1, GL_FALSE, &trafo[0][0]);
}
