#pragma once
#include "AnimatorNode.h"

class LightNode;

class BrokenLampController : public AnimatorNode
{
	LightNode* lamp_;
	glm::vec3 on_specular_color_;
	glm::vec3 on_diffuse_color_;
	double brightness_;
public:
	BrokenLampController(std::string name, LightNode *lamp);
	~BrokenLampController();

	void update(double delta) override;
	void set_brightness(double brightness);
};
