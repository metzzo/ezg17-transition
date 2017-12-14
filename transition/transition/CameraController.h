#pragma once
#include "AnimatorNode.h"
#include <iostream>

class RenderingNode;

class CameraController : public AnimatorNode {

private:
	RenderingNode *camera_;
	const float speed_ = 10;
	const float sensitivity_ = 0.5;
	double cursorX_ = 0;
	double cursorY_ = 0;
	float verAngle_ = 0;

public:
	CameraController(const std::string name, RenderingNode *camera);
	void update(double delta) override;

};