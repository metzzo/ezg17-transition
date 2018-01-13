#pragma once
#include "CameraSplineController.h"
#include "RenderingEngine.h"

class CullOffAction : public IKeyPointAction
{

public:

	explicit CullOffAction()
	{
	}

	void update(CameraSplineController* controller, float delta, float tween) override
	{
		glDisable(GL_CULL_FACE);
		RE_CULLING = false;
	}
};

