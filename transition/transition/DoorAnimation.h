#pragma once
#include "AnimatorNode.h"

class DoorAnimation : public AnimatorNode {

private:
	Node* door1_;
	glm::vec3 pos_;
	float angle1_ = 0;
	bool positive_dir_ = true;
	float offset_ = 0;
	bool enabled_ = false;

public:

	explicit DoorAnimation(std::string name, Node* door1, glm::vec3 pos, float offset = 0, bool positive_dir = true) : AnimatorNode(name)
	{
		this->door1_ = door1;
		this->pos_ = pos;
		this->offset_ = offset;
		this->positive_dir_ = positive_dir;
	}

	void start_if_not_automatic() override {
		enabled_ = true;
	}

	void update(double delta) override {
		if (!enabled_) return;
		if (offset_ > 0) {
			offset_ -= delta;
		}
		if (offset_ <= 0 && angle1_ < 90) {
			angle1_ += delta * 45;
			this->door1_->apply_transformation(Transformation::rotate_around_point((positive_dir_ * 2 - 1) * 45 * delta, glm::vec3(0, 1, 0), pos_));
		}
	}

};