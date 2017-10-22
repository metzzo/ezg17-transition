#pragma once
#include <vector>
#include <string>
using namespace std;

class IDrawable;
class RenderingEngine;
class RenderingNode;

class Node
{
	Node *parent_;
	RenderingEngine *rendering_engine_;
	string name_;

public:
	explicit Node(const string& name);
	virtual ~Node();

	virtual void init(RenderingEngine *rendering_engine);

	virtual std::vector<IDrawable*> get_drawables();
	virtual std::vector<RenderingNode*> get_rendering_nodes();

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

	string get_name() {
		return this->name_;
	}
};

