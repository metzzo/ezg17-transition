#pragma once
#include "CameraSplineController.h"
#include "BrokenLampController.h"

class LightKeyPointAction : public IKeyPointAction
{
	BrokenLampController *lamp_controller_;
	float progress_;
	CameraNode *cam_;
public:



	explicit LightKeyPointAction(BrokenLampController *controller, CameraNode *cam)
	{
		this->lamp_controller_ = controller;
		this->cam_ = cam;
		this->progress_ = 0;
	}

	void update(CameraSplineController* controller, float delta, float tween) override
	{
		if (progress_ < 4.25)
		{
			this->lamp_controller_->set_brightness(0.0);
		}
		else {
			this->lamp_controller_->set_brightness(glm::mix(0.2, 1.0, std::min(((progress_ - 4.25f) / 5)*((progress_ - 4.25f) / 5), 1.0f)));
		}

		this->progress_ += delta;
	}
};
