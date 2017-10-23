#include "CameraNode.h"
#include "RenderingEngine.h"
#include "MainShader.h"
#include "LightNode.h"

CameraNode::CameraNode(const std::string& name, const glm::ivec2& viewport, const glm::mat4& projection) : RenderingNode(name, viewport, projection)
{
}

CameraNode::~CameraNode()
{
}

void CameraNode::before_render(const std::vector<LightNode*>& light_nodes) const
{
	RenderingNode::before_render(light_nodes);

	const auto shader = this->get_rendering_engine()->get_main_shader();
	const auto program_id = shader->get_resource_id();
	glUseProgram(program_id);

	shader->set_projection(this->projection_);
	shader->set_view(this->get_transformation());

	auto light_index = 0;
	for (auto& light : light_nodes)
	{
		light->apply_to_shader(shader, light_index);
		light_index++;
	}
}

void CameraNode::set_model_matrix(const glm::mat4& trafo)
{
	const auto shader = this->get_rendering_engine()->get_main_shader();
	shader->set_model(trafo);
}
