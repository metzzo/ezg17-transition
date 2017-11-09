#include "Node.h"
#include "IDrawable.h"


Node::Node(const std::string& name)
{
	this->set_parent(nullptr);
	this->name_ = name;
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

std::vector<LightNode*> Node::get_light_nodes()
{
	return std::vector<LightNode*>();
}

std::vector<AnimatorNode*> Node::get_animator_nodes()
{
	return std::vector<AnimatorNode*>();
}
