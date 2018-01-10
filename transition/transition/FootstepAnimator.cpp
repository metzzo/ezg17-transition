#include "FootstepAnimator.h"
#include <iostream>

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
		right_foot_->start_emitting();
		left_foot_->start_emitting();

		this->flash_done_ = true;
	}
	std::cout << this->progress_ << std::endl;
	if (this->progress_ >= 40.0) {
		this->animating_ = true;
	}

	if (!this->animating_)
	{
		return;
	}
	

	if (left_countdown_ == -1) {
		if (left_start_) {
			left_countdown_ = 4;
			right_countdown_ = 2;
			left_foot_->start_emitting();
		}
		else {
			right_countdown_ = 4;
			left_countdown_ = 2;
			right_foot_->start_emitting();
		}
	}
	else {
		left_countdown_ -= delta;
		right_countdown_ -= delta;
		if (left_countdown_ <= 0) {
			left_countdown_ = 4;
			left_foot_->apply_transformation(Transformation::translate(step_size_));
			left_foot_->start_emitting();
		}
		if (right_countdown_ <= 0) {
			right_countdown_ = 4;
			right_foot_->apply_transformation(Transformation::translate(step_size_));
			right_foot_->start_emitting();
		}
	}
}

void FootstepAnimator::is_animating(bool animating)
{
	this->animating_ = animating;
}
