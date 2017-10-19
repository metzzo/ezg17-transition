#pragma once
#include <vector>
#include "IDrawable.h"
#include "RenderingNode.h"
class RenderingEngine;

class Node
{
	Node *parent_;
	RenderingEngine *rendering_engine_;
public:
	explicit Node();
	virtual ~Node();

	virtual void init(RenderingEngine *rendering_engine);

	virtual std::vector<IDrawable*> get_drawables() const;
	virtual std::vector<RenderingNode*> get_rendering_nodes() const;

	RenderingEngine *get_rendering_engine() const
	{
		return this->rendering_engine_;
	}

	void set_parent(Node *parent)
	{
		this->parent_ = parent;
	}

	Node *get_parent() const
	{
		return this->parent_;
	}
};

