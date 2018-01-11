#pragma once
#include "CameraSplineController.h"
#include "RenderingEngine.h"
#include "Node.h"
#include "AnimatorNode.h"

class AnimationAction : public IKeyPointAction
{
	AnimatorNode* animation_;

public:

	explicit AnimationAction(AnimatorNode* animation)
	{
		this->animation_ = animation;
	}

	void update(CameraSplineController* controller, float delta, float tween) override
	{
		animation_->start_if_not_automatic();
	}
};

