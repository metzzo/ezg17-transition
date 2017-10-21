#pragma once
#include "GroupNode.h"

class RenderingEngine
{
	GroupNode *root_node_;
	std::vector<IDrawable*> drawables_;
	std::vector<RenderingNode*> rendering_nodes_;
public:
	RenderingEngine();
	~RenderingEngine();

	void run();

	GroupNode* get_root_node() const
	{
		return this->root_node_;
	}
};

