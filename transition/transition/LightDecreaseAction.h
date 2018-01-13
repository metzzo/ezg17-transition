#pragma once
#include "CameraSplineController.h"
#include "RenderingEngine.h"
#include "GeometryNode.h"
#include "LightNode.h"

class LightDecreaseAction : public IKeyPointAction
{
	LightNode* light_;
	float delay_ = 0;
	float duration_ = 0;
	float passed_ = 0;
	glm::vec3 startintensity_;

public:

	explicit LightDecreaseAction(LightNode* light, float duration, float delay)
	{
		this->light_ = light;
		this->duration_ = duration;
		this->delay_ = delay;
	}

	void update(CameraSplineController* controller, float delta, float tween) override
	{
		if (delay_ >= 0) {
			delay_ -= delta;
			startintensity_ = light_->get_diffuse();
			return;
		}
		passed_ += delta;
		float alpha = 1 -glm::min(passed_ / duration_, 1.0f);
		this->light_->set_color(alpha * startintensity_, alpha * startintensity_);
	}
};

