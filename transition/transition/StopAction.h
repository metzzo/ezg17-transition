#pragma once
#include "CameraSplineController.h"
#include "RenderingEngine.h"

class StopAction : public IKeyPointAction
{
	RenderingEngine* engine_;

public:

	explicit StopAction(RenderingEngine* engine)
	{
		this->engine_ = engine;
	}

	void update(CameraSplineController* controller, float delta, float tween) override
	{
		engine_->stop();
	}
};

