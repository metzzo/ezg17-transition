#pragma once
#include "LightNode.h"
class DirectionalLightNode :
	public LightNode
{
public:
	explicit DirectionalLightNode(const std::string& name);
	~DirectionalLightNode();

	int get_light_type() override
	{
		return 1;
	}
};

