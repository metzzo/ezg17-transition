#pragma once
#include "ShaderResource.h"

class TextureRenderable;

class VolumetricLightingUpSampleShader :
	public ShaderResource
{
	GLint volumetric_tex_uniform_;
	GLint scene_tex_uniform_;
public:
	VolumetricLightingUpSampleShader();
	~VolumetricLightingUpSampleShader();


	void init() override;

	void set_camera_uniforms(const RenderingNode *node) override;
	void set_model_uniforms(const GeometryNode *node) override;
	void set_volumetric_texture(TextureRenderable *tex) const;
	void set_scene_texture(TextureRenderable *tex) const;
};

