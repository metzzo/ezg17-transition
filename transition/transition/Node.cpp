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

std::vector<IDrawable*> Node::get_drawables()
{
	return std::vector<IDrawable*>();
}

std::vector<RenderingNode*> Node::get_rendering_nodes()
{
	return std::vector<RenderingNode*>();
}
