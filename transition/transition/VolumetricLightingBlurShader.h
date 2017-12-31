#pragma once
#include "ShaderResource.h"
#include "TextureRenderable.h"

class VolumetricLightingBlurShader :
	public ShaderResource
{
	GLint scene_tex_uniform_;
	GLint volumetric_tex_uniform_;
public:
	VolumetricLightingBlurShader();
	~VolumetricLightingBlurShader();

	void init() override;

	void set_camera_uniforms(const RenderingNode *node) override;
	void set_model_uniforms(const GeometryNode *node) override;
	void set_scene_texture(TextureRenderable *tex) const;
	void set_volumetric_texture(TextureRenderable *tex) const;
};

