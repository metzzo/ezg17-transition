#include "CameraNode.h"
#include "RenderingEngine.h"
#include "ShaderResource.h"

CameraNode::CameraNode(const string& name, const glm::ivec2& viewport, const glm::mat4& projection) : RenderingNode(name, viewport, projection)
{
}

CameraNode::~CameraNode()
{
}

void CameraNode::before_render() const
{
	RenderingNode::before_render();

	const auto program_id = this->get_rendering_engine()->get_main_shader()->get_resource_id();
	glUseProgram(program_id);
}
