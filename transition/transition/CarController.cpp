#include "CarController.h"
#include "TransformationNode.h"
#include <glm/gtx/spline.hpp>
#include <iostream>

static const glm::vec2 car_positions[] = {
	glm::vec2(-20, 0),
	glm::vec2(-18, 0),
	glm::vec2(-14, 0),
	glm::vec2(-14, 10),
	glm::vec2(-14, -10),
	glm::vec2(-20, 0),
};
#define CAR_POSITIONS (6)

CarController::CarController(std::string name, TransformationNode *moving) : AnimatorNode(name)
{
	this->moving_ = moving;
	this->progress_ = 0.0;
	this->last_progress_ = 0.0;
	this->segment_ = 1;
	this->last_segment_ = 1;
}


CarController::~CarController()
{
}

void CarController::update(double delta)
{
	const auto eye = glm::catmullRom(
		car_positions[(this->last_segment_ - 1) % CAR_POSITIONS],
		car_positions[(this->last_segment_) % CAR_POSITIONS],
		car_positions[(this->last_segment_ + 1) % CAR_POSITIONS],
		car_positions[(this->last_segment_ + 2) % CAR_POSITIONS],
		this->last_progress_
	);
	const auto center = glm::catmullRom(
		car_positions[(this->segment_ - 1) % CAR_POSITIONS],
		car_positions[(this->segment_) % CAR_POSITIONS],
		car_positions[(this->segment_ + 1) % CAR_POSITIONS],
		car_positions[(this->segment_ + 2) % CAR_POSITIONS],
		this->progress_
	);
	const auto mat = glm::lookAt(
		glm::vec3(eye.x, 7.5, eye.y), 
		glm::vec3(center.x, 7.5, center.y), 
		glm::vec3(0, 1, 0));
	this->moving_->set_transformation(glm::inverse(mat), mat);

	this->last_segment_ = this->segment_;
	this->last_progress_ = this->progress_;
	this->progress_ += 0.25*delta;

	while (this->progress_ >= 1.0)
	{
		this->progress_ -= 1.0;
		this->segment_ = this->segment_ + 1;
	}

	//std::cout << "Progress " << this->progress_ << " Segment: " << this->segment_ << std::endl;
}
