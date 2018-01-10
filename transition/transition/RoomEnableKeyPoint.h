#pragma once
#include "CameraSplineController.h"
#include "RenderingEngine.h"

class RoomEnableKeyPointAction : public IKeyPointAction
{
	int room_;
	bool enable_;
public:

	explicit RoomEnableKeyPointAction(int room, bool enable)
	{
		this->room_ = room;
		this->enable_ = enable;
	}

	void update(CameraSplineController* controller, float delta, float tween) override
	{
		controller->get_rendering_engine()->set_room_enabled(room_, enable_);
	}
};

