#pragma once
#include "CameraSplineController.h"
#include "RenderingEngine.h"

class ClearColorAction : public IKeyPointAction
{
	glm::vec4 color_;

public:

	explicit ClearColorAction(glm::vec4 color)
	{
		this->color_ = color;
	}

	void update(CameraSplineController* controller, float delta, float tween) override
	{
		glClearColor(color_.x, color_.y, color_.z, color_.w);
	}
};

