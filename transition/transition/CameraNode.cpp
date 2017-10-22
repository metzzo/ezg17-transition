#include "CameraNode.h"

CameraNode::CameraNode(const string& name, const glm::ivec2& viewport, const glm::mat4& projection) : RenderingNode(name, viewport, projection)
{
}

CameraNode::~CameraNode()
{
}
