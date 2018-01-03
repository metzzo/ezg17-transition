#pragma once
#include "ShaderResource.h"
#include "TextureFBO.h"

class TextureRenderable;

class VolumetricLightingDownSampleShader :
	public ShaderResource
{
	GLint depth_tex_uniform_;
public:
	VolumetricLightingDownSampleShader();
	~VolumetricLightingDownSampleShader();

	void init() override;
	void set_camera_uniforms(const RenderingNode* node) override;
	void set_model_uniforms(const GeometryNode* node) override;

	void set_depth_texture(TextureRenderable *depth) const;
};

