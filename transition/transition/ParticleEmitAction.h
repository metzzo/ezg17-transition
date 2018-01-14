#pragma once
#include "CameraSplineController.h"
#include "ParticleEmitterNode.h"

class ParticleEmitAction : public IKeyPointAction
{
	ParticleEmitterNode* emitter_;
	float delay_;
	bool done = false;

public:

	explicit ParticleEmitAction(ParticleEmitterNode* emitter, float delay = 0)
	{
		this->emitter_ = emitter;
		this->delay_ = delay;
	}

	void update(CameraSplineController* controller, float delta, float tween) override
	{
		if (delay_ > 0) {
			delay_ -= delta;
		}
		if (delay_ <= 0 && !done) {
			emitter_->start_emitting();
			done = true;
		}
	}
};

