#include "CarController.h"
#include "LightNode.h"
#include <glm/gtx/spline.hpp>
#include <iostream>
#include <algorithm>
#include "RenderingEngine.h"

CarController::CarController(std::string name, LightNode *moving) : AnimatorNode(name)
{
	this->moving_ = moving;
	this->progress_ = 4.5;

	this->diff_color_ = this->moving_->get_diffuse();
	this->spec_color_ = this->moving_->get_specular();

	this->sound_source_ = nullptr;
	this->sound_ = nullptr;
	this->sound_play_counter_ = 0;
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


#ifdef PLAY_SOUND
	if (this->sound_play_counter_ < 5 && (this->progress_ > 10 && (this->sound_ == nullptr || this->sound_->isFinished()))) {
		this->sound_play_counter_++;
		if (this->sound_ != nullptr)
		{
			this->sound_->drop();
		}
		this->sound_ = this->get_rendering_engine()->get_sound_engine()->play2D(this->sound_source_, false, false, true);
		this->sound_->setVolume(0.025);
	}
#endif

	this->progress_ = fmod(this->progress_ + delta, 15);
}

void CarController::init(RenderingEngine* rendering_engine)
{
	AnimatorNode::init(rendering_engine);

	// load sound
	this->sound_source_ = rendering_engine->get_sound_engine()->addSoundSourceFromFile("assets/sfx/car.wav", irrklang::ESM_AUTO_DETECT, true);
}
