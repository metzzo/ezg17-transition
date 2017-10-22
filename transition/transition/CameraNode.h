#pragma once
#include "RenderingNode.h"

class CameraNode :
	public RenderingNode
{
public:
	CameraNode(const string& name, const glm::ivec2& viewport, const glm::mat4& projection);
	~CameraNode();

	void before_render() const override;
};

