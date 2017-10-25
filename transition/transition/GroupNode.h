#pragma once
#include <vector>
#include "Node.h"
#include <glm\glm.hpp>

class GroupNode : public Node
{
	std::vector<Node*> nodes_;
public:

	GroupNode(const std::string& name);
	~GroupNode();

	void init(RenderingEngine* rendering_engine) override;

	std::vector<IDrawable*> get_drawables() override;
	std::vector<RenderingNode*> get_rendering_nodes() override;

	const std::vector<Node*>& get_nodes() const;
	void add_node(Node *node);

	void apply_transformation(const glm::mat4& transformation, const glm::mat4& inverseTransformation) override;

	Node* find_by_name(const std::string& name) override;
};

