#pragma once
#include "CameraSplineController.h"
#include "RenderingEngine.h"
#include "Node.h"
#include "AnimatorNode.h"

class AnimationAction : public IKeyPointAction
{
	AnimatorNode* animation_;
	float delay_;

public:

	explicit AnimationAction(AnimatorNode* animation, float delay = 0)
	{
		this->animation_ = animation;
		this->delay_ = delay;
	}

	void update(CameraSplineController* controller, float delta, float tween) override
	{
		if (this->delay_ >= 0) {
			delay_ -= delta;
		}
		if (this->delay_ < 0) {
			animation_->start_if_not_automatic();
		}
	}
};

