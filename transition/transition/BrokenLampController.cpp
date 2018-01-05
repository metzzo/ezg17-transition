
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
	auto time = glfwGetTime()/25.0;
	double val = glm::perlin(glm::vec2(time, sin(time)*100));
	val = val*val;
	this->lamp_->set_color(
		mix(on_diffuse_color_, on_diffuse_color_*0.3f, val), 
		mix(on_specular_color_, on_specular_color_*0.3f, val)
	);
}
