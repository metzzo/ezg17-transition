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
		if (progress_ < 4)
		{
			this->lamp_controller_->set_brightness(0.0);
		}
		else {
			this->lamp_controller_->set_brightness(glm::mix(0.2, 1.0, std::min(((progress_ - 4) / 5)*((progress_ - 4) / 5), 1.0f)));

			auto bloom = glm::mix(0.0, 0.8, std::min((progress_ - 4) / 10, 1.0f));
			bloom = bloom*bloom;
			cam_->set_bloom_params(2, bloom, 0.8 - bloom);
		}

		this->progress_ += delta;
	}
};
