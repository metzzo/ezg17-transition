#include "VolumetricLightingUpSampleShader.h"
#include "TextureRenderable.h"
#include <cassert>

VolumetricLightingUpSampleShader::VolumetricLightingUpSampleShader() : ShaderResource("assets/shaders/volumetric_lighting_upsample.vs", "assets/shaders/volumetric_lighting_upsample.fs")
{
	this->scene_tex_uniform_ = -1;
	this->volumetric_tex_uniform_ = -1;
	this->bloom_treshold_uniform_ = -1;
	this->far_plane_uniform_ = -1;
	this->near_plane_uniform_ = -1;
}


VolumetricLightingUpSampleShader::~VolumetricLightingUpSampleShader()
{
}

void VolumetricLightingUpSampleShader::init()
{
	ShaderResource::init();


	this->scene_tex_uniform_ = get_uniform("scene_tex");
	this->volumetric_tex_uniform_ = get_uniform("volumetric_tex");
	this->bloom_treshold_uniform_ = get_uniform("bloom_treshold");
	this->near_plane_uniform_ = get_uniform("near_plane");
	this->far_plane_uniform_ = get_uniform("far_plane");
}

void VolumetricLightingUpSampleShader::set_camera_uniforms(const RenderingNode* node)
{
}

void VolumetricLightingUpSampleShader::set_model_uniforms(const GeometryNode* node)
{
}

void VolumetricLightingUpSampleShader::set_volumetric_texture(TextureRenderable* tex) const
{
	assert(volumetric_tex_uniform_ >= 0);
	tex->bind(0);
	glUniform1i(volumetric_tex_uniform_, 0);
}

void VolumetricLightingUpSampleShader::set_scene_texture(TextureRenderable* tex) const
{
	assert(scene_tex_uniform_ >= 0);
	tex->bind(1);
	glUniform1i(scene_tex_uniform_, 1);
}

void VolumetricLightingUpSampleShader::set_bloom_treshold(float treshold) const
{
	assert(bloom_treshold_uniform_ >= 0);
	glUniform1f(bloom_treshold_uniform_, treshold);
}

void VolumetricLightingUpSampleShader::set_near_far_plane(const float near_plane, const float far_plane) const
{
	assert(near_plane_uniform_ >= 0);
	assert(far_plane_uniform_ >= 0);
	glUniform1f(near_plane_uniform_, near_plane);
	glUniform1f(far_plane_uniform_, far_plane);
}
