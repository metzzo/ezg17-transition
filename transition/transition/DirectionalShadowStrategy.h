#pragma once
#include "LightNode.h"

class DirectionalShadowStrategy : public IShadowStrategy
{
	int shadow_map_size_;
	GLuint depth_map_fbo_;
	GLuint depth_map_;
	float near_plane_;
	float far_plane_;

public:
	explicit DirectionalShadowStrategy(int shadow_map_size, float near_plane = 1.0f, float far_plane = 100.0f);
	~DirectionalShadowStrategy();

	void init(LightNode *light_node) override;
	void before_render(const LightNode *light_node) override;
	void after_render(const LightNode *light_node) override;

	ShaderResource* get_shader(const LightNode *light_node) override;
	int get_resource_id() const override;
};

