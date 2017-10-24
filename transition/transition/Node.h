#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

class IDrawable;
class RenderingEngine;
class RenderingNode;

class Node
{
	Node *parent_;
	RenderingEngine *rendering_engine_;
	std::string name_;

public:
	explicit Node(const std::string& name);
	virtual ~Node();

	virtual void init(RenderingEngine *rendering_engine);

	virtual std::vector<IDrawable*> get_drawables();
	virtual std::vector<RenderingNode*> get_rendering_nodes();

	/*
	Applies a transformation to the Node. The inverse transformation is given as well, as this might be used for the normal-transformation-matrix 
	or the view-matrix (or not at all), depending on the type of the node
	*/
	virtual void apply_transformation(const glm::mat4& transformation, const glm::mat4& inverseTransformation) = 0;

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

	std::string get_name() const
	{
		return this->name_;
	}
};

