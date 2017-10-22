#include "MainShader.h"


MainShader::MainShader() : ShaderResource("assets/shaders/main_shader.vs", "assets/shaders/main_shader.fs")
{
	this->model_uniform_ = -1;
	this->view_uniform_ = -1;
	this->projection_uniform_ = -1;
}

void MainShader::set_model(const glm::mat4& model) const
{
	assert(this->model_uniform_ >= 0);
	glUniformMatrix4fv(this->model_uniform_, 1, GL_FALSE, &model[0][0]);
}

void MainShader::set_view(const glm::mat4& view) const
{
	assert(this->view_uniform_ >= 0);
	glUniformMatrix4fv(this->model_uniform_, 1, GL_FALSE, &view[0][0]);
}

void MainShader::set_projection(const glm::mat4& projection) const
{
	assert(this->projection_uniform_ >= 0);
	glUniformMatrix4fv(this->model_uniform_, 1, GL_FALSE, &projection[0][0]);
}

MainShader::~MainShader()
{
}

void MainShader::init()
{
	ShaderResource::init();

	// extract uniforms
	this->model_uniform_ = glGetUniformLocation(this->ShaderResource::get_resource_id(), "model");
	this->view_uniform_ = glGetUniformLocation(this->ShaderResource::get_resource_id(), "view");
	this->projection_uniform_ = glGetUniformLocation(this->ShaderResource::get_resource_id(), "projection");
}
