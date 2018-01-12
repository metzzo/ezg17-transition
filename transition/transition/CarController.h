#pragma once
#include "AnimatorNode.h"
#include <irrKlang/ik_ISoundEngine.h>
class LightNode;
class TransformationNode;

class CarController :
	public AnimatorNode
{
	LightNode *moving_;
	double progress_;

	glm::vec3 diff_color_;
	glm::vec3 spec_color_;
	irrklang::ISoundSource* sound_source_;
	irrklang::ISound* sound_;
	int sound_play_counter_;
public:
	explicit CarController(std::string name, LightNode *moving);
	~CarController();

	void update(double delta) override;
	void init(RenderingEngine* rendering_engine) override;
};

