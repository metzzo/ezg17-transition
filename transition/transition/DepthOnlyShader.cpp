#include "DepthOnlyShader.h"
#include <cassert>
#include "RenderingNode.h"
#include "GeometryNode.h"


DepthOnlyShader::DepthOnlyShader() : ShaderResource("assets/shaders/depth_shader.vs", "assets/shaders/depth_shader.fs")
{
	this->model_uniform_ = -1;
	this->view_uniform_ = -1;
	this->projection_uniform_ = -1;
}

DepthOnlyShader::~DepthOnlyShader()
{
}

void DepthOnlyShader::init()
{
	ShaderResource::init();

	this->model_uniform_ = get_uniform("mvp.model");
	this->view_uniform_ = get_uniform("mvp.view");
	this->projection_uniform_ = get_uniform("mvp.projection");

}

void DepthOnlyShader::set_camera_uniforms(const RenderingNode* node)
{
	assert(this->view_uniform_ >= 0);
	assert(this->projection_uniform_ >= 0);

	glUniformMatrix4fv(this->view_uniform_, 1, GL_FALSE, &node->get_view_matrix()[0][0]);
	glUniformMatrix4fv(this->projection_uniform_, 1, GL_FALSE, &node->get_projection_matrix()[0][0]);

}

void DepthOnlyShader::set_model_uniforms(const GeometryNode* node)
{
	assert(this->model_uniform_ >= 0);

	glUniformMatrix4fv(this->model_uniform_, 1, GL_FALSE, &node->get_transformation()[0][0]);
}
