#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Transformation.h"

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

	/*
	TODO: I wanted to call this function apply_transformation as well, however somehow other classes couldn't find the function like that
	*/
	void apply_transformation_by_object(const Transformation& transformation) {
		apply_transformation(transformation.get_transformation_matrix(), transformation.get_inverse_transformation_matrix());
	}

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

	virtual Node* find_by_name(const std::string& name) {
		if (this->name_ == name) {
			return this;
		}
		else {
			return nullptr;
		}
	}
};

