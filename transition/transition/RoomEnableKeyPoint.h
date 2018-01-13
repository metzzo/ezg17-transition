#pragma once
#include "CameraSplineController.h"
#include "RenderingEngine.h"

class RoomEnableKeyPointAction : public IKeyPointAction
{
	int room_;
	bool enable_;
	float delay_;
public:

	explicit RoomEnableKeyPointAction(int room, bool enable, float delay = 0)
	{
		this->room_ = room;
		this->enable_ = enable;
		this->delay_ = delay;
	}

	void update(CameraSplineController* controller, float delta, float tween) override
	{
		delay_ -= delta;
		std::cout << delay_ << std::endl;
		if (delay_ <= 0) {
			std::cout << "DADA" << std::endl;
			controller->get_rendering_engine()->set_room_enabled(room_, enable_);
		}
	}
};

