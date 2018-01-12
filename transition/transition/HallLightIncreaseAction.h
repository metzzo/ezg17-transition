#pragma once
#include "CameraSplineController.h"
#include "RenderingEngine.h"
#include "GeometryNode.h"
#include "LightNode.h"

class HallLightIncreaseAction : public IKeyPointAction
{
	GeometryNode* bulb_;
	LightNode* light_;
	float delay_ = 0;
	float duration_ = 0;
	float passed_ = 0;

public:

	explicit HallLightIncreaseAction(GeometryNode* bulb, LightNode* light, float duration, float delay)
	{
		this->bulb_ = bulb;
		this->light_ = light;
		this->duration_ = duration;
		this->delay_ = delay;
	}

	void update(CameraSplineController* controller, float delta, float tween) override
	{
		if (delay_ >= 0) {
			delay_ -= delta;
			return;
		}
		passed_ += delta;
		float alpha = glm::min(passed_ / duration_, 1.0f);
		this->bulb_->get_editable_mesh_resource()->get_editable_material().set_ambient_color(glm::vec3(sqrt(alpha)));
		this->light_->set_color(glm::vec3(alpha), glm::vec3(alpha));
	}
};

