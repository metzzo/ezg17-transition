#pragma once
#include "AccelerateKeyPointAction.h"
#include "CameraNode.h"

class EndCreditsAction : public IKeyPointAction
{
	CameraNode* cam_;
	double progress_;
public:
	explicit EndCreditsAction(CameraNode *cam)
	{
		this->cam_ = cam;
		progress_ = 0.0;
	}

	void update(CameraSplineController* controller, float delta, float tween) override
	{
		progress_ += delta;
		
		auto intensity = glm::clamp(this->progress_ - 3.0, 0.0, 10.0) / 10.0;
		cam_->set_end_tex_intensity(intensity * intensity);
	}
};
