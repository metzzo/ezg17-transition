#pragma once
#include <vector>

class LightNode;

class ILightShader
{
public:
	virtual ~ILightShader() = default;

	virtual void set_light_uniforms(const std::vector<LightNode*>& light_nodes) = 0;
};

