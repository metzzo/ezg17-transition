#include "VolumetricLightingBlurShader.h"
#include "TextureRenderable.h"

VolumetricLightingBlurShader::VolumetricLightingBlurShader() : ShaderResource("assets/shaders/postprocess.vs", "assets/shaders/volumetric_lighting_blur.fs")
{
	this->volumetric_tex_uniform_ = -1;
	this->is_vert_uniform_ = -1;
}


VolumetricLightingBlurShader::~VolumetricLightingBlurShader()
{
}

void VolumetricLightingBlurShader::init()
{
	ShaderResource::init();

	this->volumetric_tex_uniform_ = get_uniform("volumetric_tex");
	this->is_vert_uniform_ = get_uniform("is_vert");
}

void VolumetricLightingBlurShader::set_camera_uniforms(const RenderingNode* node)
{
}

void VolumetricLightingBlurShader::set_model_uniforms(const GeometryNode* node)
{
}

void VolumetricLightingBlurShader::set_volumetric_texture(TextureRenderable* tex) const
{
	tex->bind(0);
	glUniform1i(volumetric_tex_uniform_, 0);
}

void VolumetricLightingBlurShader::set_vertical_pass(const bool is_vertical) const
{
	glUniform1i(is_vert_uniform_, is_vertical);
}
