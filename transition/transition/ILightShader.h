#pragma once
#include <vector>

class LightNode;

class ILightShader
{
public:
	virtual ~ILightShader() = default;

	virtual void set_light_uniforms(const std::vector<LightNode*>& light_nodes) = 0;


	virtual void set_directional_shadow_map_uniforms(const LightNode *light, const GLint shadow_map) = 0;
	virtual void set_omni_directional_shadow_map_uniforms(const LightNode *light, const GLint shadow_map, float far_plane, float near_plane) = 0;
};

