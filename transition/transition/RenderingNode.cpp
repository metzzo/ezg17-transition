#include "RenderingNode.h"
#include "glheaders.h"

RenderingNode::RenderingNode(const std::string& name, const glm::ivec2 viewport, const glm::mat4 projection) : TransformationNode(name)
{
	this->viewport_ = viewport;
	this->projection_ = projection;
}

RenderingNode::~RenderingNode()
{
}

std::vector<RenderingNode*> RenderingNode::get_rendering_nodes()
{
	return { this };
}

void RenderingNode::before_render(const std::vector<LightNode*>& light_nodes) const
{
	glViewport(0, 0, this->viewport_.x, this->viewport_.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderingNode::after_render() const
{
}

void RenderingNode::render(const std::vector<IDrawable*>& drawables, const std::vector<LightNode*>& light_nodes) const
{
	before_render(light_nodes);
	
	for (auto &drawable : drawables)
	{
		drawable->draw(this->get_shader());
	}

	after_render();
}

glm::mat4 RenderingNode::get_projection_matrix() const {
	return projection_;
}
glm::mat4 RenderingNode::get_view_matrix() const {
	return this->get_inverse_transformation();
}

void RenderingNode::set_view_matrix(const glm::mat4& mat) {
	this->set_transformation(glm::inverse(mat), mat);
}