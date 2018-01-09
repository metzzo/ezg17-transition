#pragma once
#include "AnimatorNode.h"
#include <iostream>

class TransformationNode;

class CameraController : public AnimatorNode {

private:
	TransformationNode *target_;
	const float speed_ = 10;
	const float sensitivity_ = 0.5;
	double cursorX_ = 0;
	double cursorY_ = 0;
	float verAngle_ = 0;
	bool n_pressed_;

public:
	CameraController(const std::string name, TransformationNode *camera);
	void update(double delta) override;

};