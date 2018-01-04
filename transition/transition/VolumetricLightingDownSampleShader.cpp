#include "VolumetricLightingDownSampleShader.h"
#include "TextureRenderable.h"
#include <cassert>

static const int depth_texture_slot = 0;

VolumetricLightingDownSampleShader::VolumetricLightingDownSampleShader() : ShaderResource("assets/shaders/volumetric_lighting_downsample.vs", "assets/shaders/volumetric_lighting_downsample.fs")
{
	this->depth_tex_uniform_ = -1;
}


VolumetricLightingDownSampleShader::~VolumetricLightingDownSampleShader()
{
}

void VolumetricLightingDownSampleShader::init()
{

	ShaderResource::init();

	this->depth_tex_uniform_ = get_uniform("depth_tex");
}

void VolumetricLightingDownSampleShader::set_camera_uniforms(const RenderingNode* node)
{
}

void VolumetricLightingDownSampleShader::set_model_uniforms(const GeometryNode* node)
{
}

void VolumetricLightingDownSampleShader::set_depth_texture(TextureRenderable* depth) const
{
	assert(depth_tex_uniform_ >= 0);
	depth->bind(depth_texture_slot);
	glUniform1i(depth_tex_uniform_, depth_texture_slot);
}
