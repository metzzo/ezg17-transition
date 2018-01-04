#pragma once
#include "AnimatorNode.h"
class TransformationNode;

class CarController :
	public AnimatorNode
{
	TransformationNode* moving_;
	double progress_;
	int segment_;
	int last_segment_;
	double last_progress_;
public:
	explicit CarController(std::string name, TransformationNode *moving);
	~CarController();

	void update(double delta) override;
};

