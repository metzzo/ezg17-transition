#include "stdafx.h"
#include "RenderingNode.h"


RenderingNode::RenderingNode()
{
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
