#include "CarController.h"
#include "LightNode.h"
#include <glm/gtx/spline.hpp>
#include <iostream>
#include <algorithm>

CarController::CarController(std::string name, LightNode *moving) : AnimatorNode(name)
{
	this->moving_ = moving;
	this->progress_ = 4.5;

	diff_color_ = this->moving_->get_diffuse();
	spec_color_ = this->moving_->get_specular();
}


CarController::~CarController()
{
}

void CarController::update(double delta)
{
	auto pos = glm::mix(glm::vec3(-25.7828, 8, -0.330354), glm::vec3(-15, 8, -0.330354), this->progress_ / 15);

	auto intensity = (std::max(this->progress_ - 11, 0.0) / 4.0) + ((4.0 - std::min(this->progress_, 4.0)) / 4.0);
	intensity *= intensity;

	glm::mat4 mat = glm::inverse(glm::lookAt(pos, glm::vec3(25.9261, 8, 2.84429), glm::vec3(0,1,0)));
	this->moving_->set_transformation(mat);

	const auto diff_color = mix(diff_color_, glm::vec3(0), intensity);
	const auto spec_color = mix(spec_color_, glm::vec3(0), intensity);

	this->moving_->set_color(
		diff_color,
		spec_color
	);

	this->progress_ = fmod(this->progress_ + delta, 15);

	//std::cout << "Progress " << this->progress_ << " Segment: " << this->segment_ << std::endl;
}
