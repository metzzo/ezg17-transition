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
		auto drawables = node->get_drawables();
		vec.insert(vec.end(), drawables.begin(), drawables.end());
	}

	return vec;
}

std::vector<RenderingNode*> GroupNode::get_rendering_nodes()
{
	auto vec = std::vector<RenderingNode*>();

	for (auto& node : get_nodes())
	{
		auto rendering_nodes = node->get_rendering_nodes();
		vec.insert(vec.end(), rendering_nodes.begin(), rendering_nodes.end());
	}

	return vec;
}

std::vector<LightNode*> GroupNode::get_light_nodes()
{
	auto vec = std::vector<LightNode*>();

	for (auto& node : get_nodes())
	{
		auto lights = node->get_light_nodes();
		vec.insert(vec.end(), lights.begin(), lights.end());
	}

	return vec;
}

std::vector<AnimatorNode*> GroupNode::get_animator_nodes()
{
	auto vec = std::vector<AnimatorNode*>();

	for (auto& node : get_nodes())
	{
		auto animator_nodes = node->get_animator_nodes();
		vec.insert(vec.end(), animator_nodes.begin(), animator_nodes.end());
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

void GroupNode::apply_transformation(const glm::mat4& transformation, const glm::mat4& inverse_transformation) {
	for (auto& node : get_nodes()) {
		node->apply_transformation(transformation, inverse_transformation);
	}
}

Node* GroupNode::find_by_name(const std::string& name) {
	if (this->get_name() == name) {
		return this;
	}
	for (auto& node : get_nodes()) {
		Node* found = node->find_by_name(name);
		if (found != nullptr) {
			return found;
		}
	}
	return nullptr;
}