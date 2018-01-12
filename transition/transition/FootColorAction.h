#pragma once
#include "CameraSplineController.h"
#include "RenderingEngine.h"
#include "FootstepNode.h"

class FootColorAction : public IKeyPointAction
{
	FootstepNode* node_;
	glm::vec3 color_;
	float delay_;

public:

	explicit FootColorAction(FootstepNode* node, glm::vec3 color, float delay = 0)
	{
		node_ = node;
		color_ = color;
		delay_ = delay;
	}

	void update(CameraSplineController* controller, float delta, float tween) override
	{
		if (delay_ > 0) {
			delay_ -= delta;
		}
		if (delay_ <= 0) {
			node_->set_color(color_);
		}
	}
};

