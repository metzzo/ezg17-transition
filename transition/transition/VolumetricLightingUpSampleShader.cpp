#include "VolumetricLightingUpSampleShader.h"
#include "TextureRenderable.h"
#include <cassert>

VolumetricLightingUpSampleShader::VolumetricLightingUpSampleShader() : ShaderResource("assets/shaders/volumetric_lighting_upsample.vs", "assets/shaders/volumetric_lighting_upsample.fs")
{
	this->scene_tex_uniform_ = -1;
	this->volumetric_tex_uniform_ = -1;
}


VolumetricLightingUpSampleShader::~VolumetricLightingUpSampleShader()
{
}

void VolumetricLightingUpSampleShader::init()
{
	ShaderResource::init();


	this->scene_tex_uniform_ = get_uniform("scene_tex");
	this->volumetric_tex_uniform_ = get_uniform("volumetric_tex");
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