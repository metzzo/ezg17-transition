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
#ifdef VISUALIZE_KEYPOINTS
	//this->progress_ = 38;
#endif
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

	//std::cout << progress_ << std::endl;
	
	if (progress_ >= 110 && stage == 1) {
		//Richtungswechsel
		//Achtung, schirches Hardcoden
		glm::vec3 leftaim = glm::vec3(-1.2, 0, -22.5);
		glm::vec3 rightaim = glm::vec3(-0.2, 0, -20.5);
		glm::vec3 leftnow = left_foot_->get_position();
		glm::vec3 rightnow = right_foot_->get_position();
		left_foot_->apply_transformation(Transformation::translate(leftaim - leftnow));
		right_foot_->apply_transformation(Transformation::translate(rightaim - rightnow));
		left_foot_->apply_transformation(Transformation::rotate_around_point(-90, glm::vec3(0, 1, 0), left_foot_->get_position()));
		right_foot_->apply_transformation(Transformation::rotate_around_point(-90, glm::vec3(0, 1, 0), right_foot_->get_position()));
		//std::cout << left_foot_->get_position().x << "," << left_foot_->get_position().y << "," << left_foot_->get_position().z << std::endl;
		//std::cout << right_foot_->get_position().x << "," << right_foot_->get_position().y << "," << right_foot_->get_position().z << std::endl;
		step_size_ = glm::vec3(3, 0, 0);
		foot_speed_ /= 1.5;//das ist eigentlich keine speed, sondern die länge (verwirrende benennung)
		left_countdown_ = foot_speed_;
		right_countdown_ = foot_speed_*2;
		stage = 2;
	}

	if (left_countdown_ == -1) {
		if (left_start_) {
			left_countdown_ = 2* foot_speed_;
			right_countdown_ = foot_speed_;
			left_foot_->start_emitting();
		}
		else {
			right_countdown_ = 2* foot_speed_;
			left_countdown_ = foot_speed_;
			right_foot_->start_emitting();
		}
	}
	else {
		left_countdown_ -= delta;
		right_countdown_ -= delta;
		if (left_countdown_ <= 0) {
			left_countdown_ = 2* foot_speed_;
			left_foot_->apply_transformation(Transformation::translate(step_size_));
			left_foot_->start_emitting();
			if (progress_ >= 114 && progress_ <= 132) {
				foot_speed_ *= 0.95;
			}
		}
		if (right_countdown_ <= 0) {
			right_countdown_ = 2* foot_speed_;
			right_foot_->apply_transformation(Transformation::translate(step_size_));
			right_foot_->start_emitting();
			if (progress_ >= 114 && progress_ <= 132) {
				foot_speed_ *= 0.95;
			}
		}
	}
}

void FootstepAnimator::is_animating(bool animating)
{
	this->animating_ = animating;
}
