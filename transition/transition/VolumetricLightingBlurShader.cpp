#include "VolumetricLightingBlurShader.h"


VolumetricLightingBlurShader::VolumetricLightingBlurShader() : ShaderResource("assets/shaders/postprocess.vs", "assets/shaders/volumetric_lighting_blur.fs")
{
	this->scene_tex_uniform_ = -1;
	this->volumetric_tex_uniform_ = -1;
}


VolumetricLightingBlurShader::~VolumetricLightingBlurShader()
{
}

void VolumetricLightingBlurShader::init()
{
	ShaderResource::init();

	this->scene_tex_uniform_ = get_uniform("scene_tex");
	this->volumetric_tex_uniform_ = get_uniform("volumetric_tex");
}

void VolumetricLightingBlurShader::set_camera_uniforms(const RenderingNode* node)
{
}

void VolumetricLightingBlurShader::set_model_uniforms(const GeometryNode* node)
{
}

void VolumetricLightingBlurShader::set_scene_texture(TextureRenderable* tex) const
{
	tex->bind(0);
	glUniform1i(volumetric_tex_uniform_, 0);
}

void VolumetricLightingBlurShader::set_volumetric_texture(TextureRenderable* tex) const
{
	tex->bind(1);
	glUniform1i(volumetric_tex_uniform_, 1);
}
