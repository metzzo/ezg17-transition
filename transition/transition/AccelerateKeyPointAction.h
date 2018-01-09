#pragma once
#include "CameraSplineController.h"

class AccelerateKeyPointAction : public IKeyPointAction
{
	double progress_;
	float standing_time_;

	float accelerate_time_;
public:

	explicit AccelerateKeyPointAction(float standing_time, float accelerate_time)
	{
		this->progress_ = 0;
		this->standing_time_ = standing_time;
		this->accelerate_time_ = accelerate_time;
	}

	void update(CameraSplineController* controller, float delta, float tween) override
	{
		this->progress_ += delta;

		if (this->progress_ >= this->standing_time_)
		{

			delta *= 1.0 - std::min((this->progress_ - this->standing_time_) / accelerate_time_, 1.0);
		}

		// reverse time
		controller->set_progress(controller->get_progress() - delta);
	}
};
