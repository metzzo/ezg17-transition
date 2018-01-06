#include "VolumetricLightingBlurShader.h"
#include "TextureRenderable.h"
#include <cassert>

VolumetricLightingBlurShader::VolumetricLightingBlurShader() : ShaderResource("assets/shaders/postprocess.vs", "assets/shaders/volumetric_lighting_blur.fs")
{
	this->volumetric_tex_uniform_ = -1;
	this->is_vert_uniform_ = -1;
	this->far_plane_uniform_ = -1;
	this->near_plane_uniform_ = -1;
}


VolumetricLightingBlurShader::~VolumetricLightingBlurShader()
{
}

void VolumetricLightingBlurShader::init()
{
	ShaderResource::init();

	this->volumetric_tex_uniform_ = get_uniform("volumetric_tex");
	this->is_vert_uniform_ = get_uniform("is_vert");
	this->near_plane_uniform_ = get_uniform("near_plane");
	this->far_plane_uniform_ = get_uniform("far_plane");
}

void VolumetricLightingBlurShader::set_camera_uniforms(const RenderingNode* node)
{
}

void VolumetricLightingBlurShader::set_model_uniforms(const GeometryNode* node)
{
}

void VolumetricLightingBlurShader::set_volumetric_texture(TextureRenderable* tex) const
{
	assert(volumetric_tex_uniform_ >= 0);
	tex->bind(0);
	glUniform1i(volumetric_tex_uniform_, 0);
}

void VolumetricLightingBlurShader::set_vertical_pass(const bool is_vertical) const
{
	assert(is_vert_uniform_ >= 0);
	glUniform1i(is_vert_uniform_, is_vertical);
}

void VolumetricLightingBlurShader::set_near_far_plane(const float near_plane, const float far_plane) const
{
	assert(near_plane_uniform_ >= 0);
	assert(far_plane_uniform_ >= 0);
	glUniform1f(near_plane_uniform_, near_plane);
	glUniform1f(far_plane_uniform_, far_plane);
}
