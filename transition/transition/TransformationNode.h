#pragma once
#include "Node.h"
#include <glm\glm.hpp>

/*
Each Node is transformable by the function "applyTransformation".
However, not each node might store a transformation matrix.
This class resembles all nodes which store a transformation matrix (and it's inverse for certain purposes such as normal transformation or view matrix calculations).
*/
class TransformationNode : public Node {
	glm::mat4 trafo_;
	glm::mat4 itrafo_;
public:
	explicit TransformationNode(const std::string& name) : Node(name) {
		trafo_ = itrafo_ = glm::mat4(1.0f);
	};
	virtual ~TransformationNode() = default;

	virtual void set_transformation(const glm::mat4& trafo, const glm::mat4& itrafo)
	{
		this->trafo_ = trafo;
		this->itrafo_ = itrafo;
	}

	virtual void set_transformation(const glm::mat4& trafo) {
		this->trafo_ = trafo;
		this->itrafo_ = glm::inverse(trafo);
	}

	void apply_transformation(const glm::mat4& mat, const glm::mat4& imat) override {
		this->trafo_ = mat * this->trafo_;
		this->itrafo_ = this->itrafo_ * imat;
	}

	//This has to be redefined, or else the other method is hiding it...
	void apply_transformation(const Transformation& transformation) override {
		Node::apply_transformation(transformation);
	}

	virtual const glm::mat4& get_transformation() const
	{
		return this->trafo_;
	}

	virtual const glm::mat4& get_inverse_transformation() const {
		return this->itrafo_;
	}

	virtual glm::vec3 get_position() const
	{
		return glm::vec3(this->trafo_[3]);
	}
};