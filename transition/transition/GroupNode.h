#pragma once
#include <vector>
#include "Node.h"

class GroupNode : public Node
{
	std::vector<Node*> nodes_;
public:

	~GroupNode();
	GroupNode();

	void init(RenderingEngine* rendering_engine) override;

	std::vector<const IDrawable*> get_drawables() const override;
	std::vector<const RenderingNode*> get_rendering_nodes() const override;

	const std::vector<Node*>& get_nodes() const;
	void add_node(Node *node);
};

