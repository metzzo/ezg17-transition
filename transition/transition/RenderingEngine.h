#pragma once
#include "GroupNode.h"

class RenderingEngine
{
	GroupNode *root_node_;
	std::vector<IDrawable*> drawables_;
	std::vector<RenderingNode*> rendering_nodes_;
	int width_;
	int height_;
public:
	RenderingEngine(int width, int height);
	~RenderingEngine();

	void run();

	GroupNode* get_root_node() const
	{
		return this->root_node_;
	}
};

