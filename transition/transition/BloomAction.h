#pragma once
#include "CameraSplineController.h"
#include "CameraNode.h"

class BloomAction : public IKeyPointAction
{
	CameraNode* camera_;
	glm::vec2 from_;
	glm::vec2 to_;
	float duration_;
	float delay_;
	float passed_ = 0;

public:

	explicit BloomAction(CameraNode* camera, glm::vec2 from, glm::vec2 to, float duration, float delay)
	{
		this->camera_ = camera;
		this->from_ = from;
		this->to_ = to;
		this->duration_ = duration;
		this->delay_ = delay;
	}

	void update(CameraSplineController* controller, float delta, float tween) override
	{
		if (delay_ > 0) {
			delay_ -= delta;
		}
		if (delay_ <= 0) {
			passed_ += delta;
			float alpha = glm::min(passed_ / duration_, 1.0f);
			glm::vec2 interp = glm::mix(from_, to_, alpha);
			camera_->set_bloom_params(1, interp.x, interp.y);
		}
	}
};

