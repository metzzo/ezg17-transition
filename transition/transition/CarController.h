#pragma once
#include "AnimatorNode.h"
class LightNode;

class CarController :
	public AnimatorNode
{
	LightNode* moving_;
	double progress_;

	glm::vec3 diff_color_;
	glm::vec3 spec_color_;
public:
	explicit CarController(std::string name, LightNode *moving);
	~CarController();

	void update(double delta) override;
};

