#include "CameraNode.h"
#include "RenderingEngine.h"
#include "MainShader.h"

CameraNode::CameraNode(const std::string& name, const glm::ivec2& viewport, const glm::mat4& projection) : RenderingNode(name, viewport, projection)
{
}

CameraNode::~CameraNode()
{
}

void CameraNode::before_render() const
{
	RenderingNode::before_render();

	const auto shader = this->get_rendering_engine()->get_main_shader();
	const auto program_id = shader->get_resource_id();
	glUseProgram(program_id);

	shader->set_camera_uniforms(this);
}

ShaderResource* CameraNode::getShader() const {
	return this->get_rendering_engine()->get_main_shader();
}