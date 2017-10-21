#pragma once
#include "GroupNode.h"
#include <glm/vec2.hpp>

class RenderingEngine
{
	GroupNode *root_node_;
	std::vector<IDrawable*> drawables_;
	std::vector<RenderingNode*> rendering_nodes_;
	glm::ivec2 viewport_;
public:
	explicit RenderingEngine::RenderingEngine(const glm::ivec2 viewport);
	~RenderingEngine();

	void run();

	GroupNode* get_root_node() const
	{
		return this->root_node_;
	}

	const glm::ivec2& get_viewport() const
	{
		return viewport_;
	}
};

