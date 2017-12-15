#pragma once
#include "LightNode.h"
class OmniDirectionalShadowStrategy :
	public IShadowStrategy
{
	GLuint depth_cubemap_;
	int shadow_map_size_;
	float near_plane_;
	float far_plane_;
	GLuint depth_cubemap_fbo_;
public:
	explicit OmniDirectionalShadowStrategy(int shadow_map_size, float near_plane = 1.0f, float far_plane = 100.0f);
	~OmniDirectionalShadowStrategy();

	void init(LightNode *light_node) override;
	void before_render(const LightNode *light_node) override;
	void after_render(const LightNode *light_node) override;
	ShaderResource *get_shader(const LightNode *light_node) override;
	int get_resource_id() const override;

	float get_far_plane() const
	{
		return this->far_plane_;
	}

	float get_near_plane() const
	{
		return this->near_plane_;
	}
};

