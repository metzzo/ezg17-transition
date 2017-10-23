#pragma once
#include <vector>
#include "Node.h"

class GroupNode : public Node
{
	std::vector<Node*> nodes_;
public:

	GroupNode(const std::string& name);
	~GroupNode();

	void init(RenderingEngine* rendering_engine) override;

	std::vector<IDrawable*> get_drawables() override;
	std::vector<RenderingNode*> get_rendering_nodes() override;
	std::vector<LightNode*> get_light_nodes() override;

	const std::vector<Node*>& get_nodes() const;
	void add_node(Node *node);
};

