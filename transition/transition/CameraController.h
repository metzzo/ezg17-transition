#pragma once
#include "AnimatorNode.h"
#include <iostream>

class TransformationNode;
class GeometryNode;
class GroupNode;
class MeshResource;

class CameraController : public AnimatorNode {

private:
	TransformationNode *target_;
	const float speed_ = 10;
	const float sensitivity_ = 0.5;
	double cursorX_ = 0;
	double cursorY_ = 0;
	float verAngle_ = 0;
	bool n_pressed_;
	bool c_pressed_;
	glm::vec3 lookat_;
	GeometryNode* look_at_marker_;
	MeshResource* mesh_;
	glm::mat4 save_trafo_[10];
public:
	CameraController(const std::string name, TransformationNode *camera, GroupNode *container);
	void update(double delta) override;

	void init(RenderingEngine* rendering_engine) override;
};