#include "FootParticleShader.h"
#include "RenderingNode.h"
#include "GeometryNode.h"

FootParticleShader::FootParticleShader() : ShaderResource("assets/shaders/foot_part.vs", "assets/shaders/foot_part.fs", "assets/shaders/foot_part.gs")
{
	model_uniform_ = -1;
	view_uniform_ = -1;
	projection_uniform_ = -1;
	size_uniform_ = -1;
}

FootParticleShader::~FootParticleShader()
{
}

void FootParticleShader::init()
{
	ShaderResource::init();
	this->model_uniform_ = get_uniform("mvp.model");
	this->view_uniform_ = get_uniform("mvp.view");
	this->projection_uniform_ = get_uniform("mvp.projection");
	this->size_uniform_ = get_uniform("size");
}

void FootParticleShader::set_camera_uniforms(const RenderingNode * node)
{
	glUniformMatrix4fv(this->view_uniform_, 1, GL_FALSE, &node->get_view_matrix()[0][0]);
	glUniformMatrix4fv(this->projection_uniform_, 1, GL_FALSE, &node->get_projection_matrix()[0][0]);
}

void FootParticleShader::set_model_uniforms(const GeometryNode * node)
{
	glUniformMatrix4fv(this->model_uniform_, 1, GL_FALSE, &node->get_transformation()[0][0]);
}

void FootParticleShader::set_modelmat_uniforms(const glm::mat4 & trafo, const glm::vec2& size)
{
	glUniformMatrix4fv(this->model_uniform_, 1, GL_FALSE, &trafo[0][0]);
	glUniform2fv(this->size_uniform_, 1, &size[0]);
}
