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

	virtual void before_render() const;
	virtual void after_render() const;
	void render(const std::vector<IDrawable*> &drawables) const;
};

