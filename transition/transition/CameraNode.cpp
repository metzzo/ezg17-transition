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

void CameraNode::before_render(const std::vector<IDrawable*> &drawables, const std::vector<LightNode*> &light_nodes) const
{
	for (auto &light : light_nodes)
	{
		light->render(drawables, std::vector<LightNode*>());
	}

	RenderingNode::before_render(drawables, light_nodes);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const auto shader = this->get_shader();
	shader->use();
	shader->set_light_uniforms(light_nodes);
	shader->set_camera_uniforms(this);
}

MainShader* CameraNode::get_shader() const {
	return this->get_rendering_engine()->get_main_shader();
}