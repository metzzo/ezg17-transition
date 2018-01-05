
#include "BrokenLampController.h"
#include "LightNode.h"
#include <glm/gtc/noise.hpp>

BrokenLampController::BrokenLampController(std::string name, LightNode* lamp) : AnimatorNode(name)
{
	this->lamp_ = lamp;
	this->on_diffuse_color_ = lamp->get_diffuse();
	this->on_specular_color_ = lamp->get_specular();
}

BrokenLampController::~BrokenLampController()
{
}

void BrokenLampController::update(double delta)
{
	const auto time = glfwGetTime();
	
	double val = glm::perlin(glm::vec2(time, 0.0));
	val *= val;

	const auto diff_color = mix(on_diffuse_color_, on_diffuse_color_*0.5f, val);
	const auto spec_color = mix(on_specular_color_, on_specular_color_*0.5f, val);

	this->lamp_->set_color(
		diff_color, 
		spec_color
	);
}
