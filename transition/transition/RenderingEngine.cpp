#include "stdafx.h"
#include "RenderingEngine.h"
#include "RenderingNode.h"

RenderingEngine::RenderingEngine()
{
	this->root_node_ = new GroupNode();
}


RenderingEngine::~RenderingEngine()
{
	delete this->root_node_;
}

void RenderingEngine::run()
{
	this->root_node_->init(this);

	this->drawables_ = this->root_node_->get_drawables();
	this->rendering_nodes_ = this->root_node_->get_rendering_nodes();

	while (true)
	{
		for (auto& rendering_node : this->rendering_nodes_)
		{
			rendering_node->render(this->drawables_);
		}
	}
}