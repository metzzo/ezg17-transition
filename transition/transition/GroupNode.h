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

	const std::vector<Node*>& get_nodes() const;
	void add_node(Node *node);
};

