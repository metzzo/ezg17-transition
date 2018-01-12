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
	bool close_again_ = false;

public:

	explicit DoorAnimation(std::string name, Node* door1, glm::vec3 pos, float offset = 0, bool positive_dir = true, bool close_again = false) : AnimatorNode(name)
	{
		this->door1_ = door1;
		this->pos_ = pos;
		this->offset_ = offset;
		this->positive_dir_ = positive_dir;
		this->close_again_ = close_again;
	}

	void start_if_not_automatic() override {
		enabled_ = true;
	}

	void update(double delta) override {
		if (!enabled_) return;
		if (offset_ > 0) {
			offset_ -= delta;
			return;
		}
		if (angle1_ < 90) {
			angle1_ += delta * 45;
			this->door1_->apply_transformation(Transformation::rotate_around_point((positive_dir_ * 2 - 1) * 45 * delta, glm::vec3(0, 1, 0), pos_));
		}
		else if (close_again_) {
			angle1_ += delta * 45;
			if (angle1_ > 250 && angle1_ < 340) {
				this->door1_->apply_transformation(Transformation::rotate_around_point(-(positive_dir_ * 2 - 1) * 45 * delta, glm::vec3(0, 1, 0), pos_));
			}
		}
	}

};