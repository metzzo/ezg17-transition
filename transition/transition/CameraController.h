#pragma once
#include "AnimatorNode.h"
#include "CameraNode.h"
#include "RenderingEngine.h"
#include <iostream>

class CameraController : public AnimatorNode {

private:
	CameraNode *camera_;
	RenderingEngine *engine_;
	const float speed_ = 10;
	const float sensitivity_ = 0.5;
	double cursorX_ = 0;
	double cursorY_ = 0;
	float verAngle_ = 0;

public:
	CameraController(const std::string name, CameraNode *camera, RenderingEngine *engine);
	void update(double delta) override;

};