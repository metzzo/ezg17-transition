#pragma once
#include "ShaderResource.h"

class TextureRenderable;

class VolumetricLightingUpSampleShader :
	public ShaderResource
{
	GLint volumetric_tex_uniform_;
	GLint scene_tex_uniform_;
	GLint bloom_treshold_uniform_;
	GLint near_plane_uniform_;
	GLint far_plane_uniform_;
public:
	VolumetricLightingUpSampleShader();
	~VolumetricLightingUpSampleShader();


	void init() override;

	void set_camera_uniforms(const RenderingNode *node) override;
	void set_model_uniforms(const GeometryNode *node) override;
	void set_volumetric_texture(TextureRenderable *tex) const;
	void set_scene_texture(TextureRenderable *tex) const;
	void set_bloom_treshold(float treshold) const;
	void set_near_far_plane(const float near_plane, const float far_plane) const;
};

