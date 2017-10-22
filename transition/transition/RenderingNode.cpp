#include "RenderingNode.h"
#include "glheaders.h"

RenderingNode::RenderingNode(const glm::ivec2 viewport, const glm::mat4 projection)
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

void RenderingNode::before_render() const
{
	glViewport(0, 0, this->viewport_.x, this->viewport_.y);
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderingNode::after_render() const
{
}

void RenderingNode::render(const std::vector<IDrawable*>& drawables) const
{
	before_render();
	
	for (auto &drawable : drawables)
	{
		drawable->draw();
	}

	after_render();
}
