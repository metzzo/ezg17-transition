#pragma once
#include "RenderingNode.h"

class CameraNode :
	public RenderingNode
{
public:
	CameraNode(const glm::ivec2& viewport, const glm::mat4& projection);
	~CameraNode();
};

