#pragma once
#include "CameraSplineController.h"
#include "FootstepAnimator.h"

class ShowFeetKeyPointAction : public IKeyPointAction
{
	FootstepAnimator *animator_;
public:

	explicit ShowFeetKeyPointAction(FootstepAnimator *animator)
	{
		this->animator_ = animator;
	}

	void update(CameraSplineController* controller, float delta, float tween) override
	{
		if (controller->get_progress() >= 60 + 6 - 12) {
			this->animator_->is_animating(true);
		}
	}
};
