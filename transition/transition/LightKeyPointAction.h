#pragma once
#include "CameraSplineController.h"
#include "BrokenLampController.h"

class LightKeyPointAction : public IKeyPointAction
{
	BrokenLampController *lamp_controller_;
	float progress_;
	CameraNode *cam_;
	bool quake_;
public:



	explicit LightKeyPointAction(BrokenLampController *controller, CameraNode *cam)
	{
		this->lamp_controller_ = controller;
		this->cam_ = cam;
		this->progress_ = 0;
		this->quake_ = false;
	}

	void update(CameraSplineController* controller, float delta, float tween) override
	{
		if (progress_ < 4.25)
		{
			this->lamp_controller_->set_brightness(0.0);
		}
		else {
			if (!quake_) {
				quake_ = true;
				this->lamp_controller_->quake(12.5, 7.5, false);
			}
			this->lamp_controller_->set_brightness(glm::mix(0.2, 1.0, std::min(((progress_ - 4.25f) / 5)*((progress_ - 4.25f) / 5), 1.0f)));
		}

		this->progress_ += delta;
	}
};
