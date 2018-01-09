#include "DirectionalDepthShader.h"
#include <cassert>
#include "RenderingNode.h"
#include "GeometryNode.h"
#include "TextureResource.h"


DirectionalDepthShader::DirectionalDepthShader() : ShaderResource("assets/shaders/depth_shader_directional.vs", "assets/shaders/depth_shader_directional.fs")
{
	this->mvp_uniform_ = -1;
}

DirectionalDepthShader::~DirectionalDepthShader()
{
}

void DirectionalDepthShader::init()
{
	ShaderResource::init();

	this->mvp_uniform_ = get_uniform("mvp");
	this->has_alpha_tex_uniform_ = get_uniform("has_alpha_tex");
	this->alpha_tex_uniform_ = get_uniform("alpha_tex");

}

void DirectionalDepthShader::set_camera_uniforms(const RenderingNode* node)
{
	assert(this->mvp_uniform_ >= 0);

	this->view_projection_ = node->get_projection_matrix()*node->get_view_matrix();

}

void DirectionalDepthShader::set_model_uniforms(const GeometryNode* node)
{
	assert(this->mvp_uniform_ >= 0);
	assert(this->has_alpha_tex_uniform_ >= 0);
	assert(this->alpha_tex_uniform_ >= 0);
	auto mvp = this->view_projection_*node->get_transformation();
	glUniformMatrix4fv(this->mvp_uniform_, 1, GL_FALSE, &mvp[0][0]);
	Material mat = node->get_mesh_resource()->get_material();
	if (mat.has_alpha_texture()) {
		glUniform1i(this->has_alpha_tex_uniform_, 1);
		mat.get_alpha_texture()->bind(0);
		glUniform1i(this->alpha_tex_uniform_, 0);
	}
	else {
		glUniform1i(this->has_alpha_tex_uniform_, 0);
	}
}
