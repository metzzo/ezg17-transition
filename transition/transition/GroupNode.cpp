#include "stdafx.h"
#include "GroupNode.h"
#include <cassert>

GroupNode::~GroupNode()
{
	for (auto &node : this->nodes_)
	{
		delete node;
	}
}

GroupNode::GroupNode()
{
}

void GroupNode::init(RenderingEngine* rendering_engine)
{
	Node::init(rendering_engine);

	for (auto& nodes : get_nodes())
	{
		nodes->init(rendering_engine);
	}
}

std::vector<const IDrawable*> GroupNode::get_drawables() const
{
	auto vec = std::vector<const IDrawable*>();

	for (auto& node : get_nodes())
	{
		auto node_drawables = node->get_drawables();
		vec.insert(vec.end(), node_drawables.begin(), node_drawables.end());
	}

	return vec;
}

std::vector<const RenderingNode*> GroupNode::get_rendering_nodes() const
{
	auto vec = std::vector<const RenderingNode*>();

	for (auto& node : get_nodes())
	{
		auto node_drawables = node->get_rendering_nodes();
		vec.insert(vec.end(), node_drawables.begin(), node_drawables.end());
	}

	return vec;
}

const std::vector<Node*>& GroupNode::get_nodes() const
{
	return nodes_;
}

void GroupNode::add_node(Node* node)
{
	assert(node != nullptr);
	assert(node->get_parent() == nullptr);

	this->nodes_.push_back(node);
	node->set_parent(this);
}
