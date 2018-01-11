#include "FootstepAnimator.h"
#include <iostream>

#define FOOT_SPEED 2.181818181818181818

FootstepAnimator::FootstepAnimator(const std::string& name, FootstepNode* left_foot, FootstepNode* right_foot, bool left_start, glm::vec3 step_size) : AnimatorNode(name)
{
	this->left_foot_ = left_foot;
	this->right_foot_ = right_foot;
	this->left_start_ = left_start;
	this->step_size_ = step_size;
	this->animating_ = false;
	this->flash_done_ = false;
	this->progress_ = 0.0;
}

void FootstepAnimator::update(double delta)
{
	progress_ += delta;

	if (progress_>= 30.5 && !this->flash_done_)
	{
		right_foot_->shine(14);
		left_foot_->shine(14);

		this->flash_done_ = true;
	}

	if (this->progress_ >= 40.0 && !this->animating_) {
		this->animating_ = true;
		//left_foot_->apply_transformation(Transformation::translate(step_size_));
		right_foot_->apply_transformation(Transformation::translate(step_size_ * (1.0f/2.0f)));
	}

	if (!this->animating_)
	{
		return;
	}
	

	if (left_countdown_ == -1) {
		if (left_start_) {
			left_countdown_ = 2*FOOT_SPEED;
			right_countdown_ = FOOT_SPEED;
			left_foot_->start_emitting();
		}
		else {
			right_countdown_ = 2*FOOT_SPEED;
			left_countdown_ = FOOT_SPEED;
			right_foot_->start_emitting();
		}
	}
	else {
		left_countdown_ -= delta;
		right_countdown_ -= delta;
		if (left_countdown_ <= 0) {
			left_countdown_ = 2*FOOT_SPEED;
			left_foot_->apply_transformation(Transformation::translate(step_size_));
			left_foot_->start_emitting();
		}
		if (right_countdown_ <= 0) {
			right_countdown_ = 2*FOOT_SPEED;
			right_foot_->apply_transformation(Transformation::translate(step_size_));
			right_foot_->start_emitting();
		}
	}
}

void FootstepAnimator::is_animating(bool animating)
{
	this->animating_ = animating;
}
