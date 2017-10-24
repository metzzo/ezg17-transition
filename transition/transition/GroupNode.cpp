#include "GroupNode.h"
#include <cassert>

GroupNode::GroupNode(const std::string& name) : Node(name)
{
}

GroupNode::~GroupNode()
{
	for (auto &node : this->nodes_)
	{
		delete node;
	}
}

void GroupNode::init(RenderingEngine* rendering_engine)
{
	Node::init(rendering_engine);

	for (auto& node : this->get_nodes())
	{
		node->init(rendering_engine);
	}
}

std::vector<IDrawable*> GroupNode::get_drawables()
{
	auto vec = std::vector<IDrawable*>();

	for (auto& node : get_nodes())
	{
		auto node_drawables = node->get_drawables();
		vec.insert(vec.end(), node_drawables.begin(), node_drawables.end());
	}

	return vec;
}

std::vector<RenderingNode*> GroupNode::get_rendering_nodes()
{
	auto vec = std::vector<RenderingNode*>();

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

void GroupNode::apply_transformation(const glm::mat4& transformation, const glm::mat4& inverseTransformation) {
	for (auto& node : get_nodes()) {
		node->apply_transformation(transformation, inverseTransformation);
	}
}