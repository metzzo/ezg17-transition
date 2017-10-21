#include "CameraNode.h"

CameraNode::CameraNode(const glm::ivec2& viewport, const glm::mat4& projection)
	: RenderingNode(viewport, projection)
{
}

CameraNode::~CameraNode()
{
}
