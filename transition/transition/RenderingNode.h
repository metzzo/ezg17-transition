#pragma once
#include "Node.h"
#include <vector>
#include "IDrawable.h"

class RenderingNode :
	public Node
{
public:
	RenderingNode();
	~RenderingNode();

	std::vector<const RenderingNode*> get_rendering_nodes() const override;

	virtual void before_render() const;
	virtual void after_render() const;
	void render(const std::vector<const IDrawable*> &drawables) const;
};

