#include "stdafx.h"
#include "Node.h"
#include "IDrawable.h"


Node::Node()
{
	this->set_parent(nullptr);
}

Node::~Node()
{
}

void Node::init(RenderingEngine* rendering_engine)
{
	this->rendering_engine_ = rendering_engine;
}

std::vector<const IDrawable*> Node::get_drawables() const
{
	return std::vector<const IDrawable*>();
}

std::vector<const RenderingNode*> Node::get_rendering_nodes() const
{
	return std::vector<const RenderingNode*>();
}
