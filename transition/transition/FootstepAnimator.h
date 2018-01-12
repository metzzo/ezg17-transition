#pragma once
#include "AnimatorNode.h"
#include "FootstepNode.h"
#include "CameraSplineController.h"

class FootstepAnimator : public AnimatorNode {

private:
	FootstepNode* left_foot_;
	FootstepNode* right_foot_;
	float left_countdown_ = -1;
	float right_countdown_ = -1;
	bool left_start_ = false;
	glm::vec3 step_size_;
	bool animating_;
	bool flash_done_;
	double progress_;
	float foot_speed_ = 2.181818181818181818;
	int stage = 1;

public:
	FootstepAnimator(const std::string& name, FootstepNode* left_foot_, FootstepNode* right_foot_, bool left_start_, glm::vec3 stepsize);
	
	void update(double delta);
	void is_animating(bool animating);
};
