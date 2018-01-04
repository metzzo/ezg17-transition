#pragma once
#include "AnimatorNode.h"

class TransformationNode;

class LookAtController : public AnimatorNode
{
	TransformationNode *target_node_;
	TransformationNode *source_node_;

public:
	LookAtController(const std::string name, TransformationNode *target_node, TransformationNode *source_node);

	void update(double delta) override;
};